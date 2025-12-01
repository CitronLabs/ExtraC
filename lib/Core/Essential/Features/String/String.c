#include "../../../../pkg.h"

import(std)


/**
 * @details This method compares the contents of two String objects. It first
 * checks for mismatches in length and encoding type. If they match, it iterates
 * through the strings, decoding codepoints from the UTF-8 representation and
 * comparing them.
*/
bool methodimpl(std_String, Compare, std_String* cmp_string){
	
	if(elements(self) != elements(cmp_string) 		|| 
	   self->type  != cmp_string->type 			||
	   priv.len_bytes != cmp_string->__private->len_bytes
	) 
		return false;

	switch (self->type) {
	case CHAR_UTF8:
	case CHAR_UTF16:
	case CHAR_UTF32:
	case CHAR_ASCII:
	default:{

	}
	}

return false;
}

std_String* methodimpl(std_String,Copy){

	std_String* res = priv.inline_alloc ?
		malloc(sizeof(std_String)) : 
		malloc(sizeof(std_String)  + priv.len_bytes)
	;

	res->__private.len_bytes = priv.len_bytes;
	res->__private.inline_alloc = priv.inline_alloc;

	res->len 	= self->len;
	res->type       = res->type,
	res->data.utf8 = priv.inline_alloc ? 
			pntr_shiftcpy(res, sizeof(std_String)) : 
			malloc(priv.len_bytes);

	memcpy(res->data.utf8, self->data.utf8, priv.len_bytes);

return res;
}

std_String* methodimpl(std_String, Cat, ...){
	
	va_list args;
	va_start(args, self);

	var builder = push(std_StringBuilder, .init_str = self);
	
	std.StringBuilder.Append(builder, args);

	std_String* result = std.StringBuilder.CreateStr(builder);

	del(builder);

return result;
}

std_String* methodimpl(std_String, Convert, std_char_type type){
	nonull(self, return nil);
	
	// If the target encoding is the same as the source, just return a copy.
	if (self->type == type) {
		return std.String.Copy(self);
	}

	len_t init_allocLen = (self->len + 1) * sizeof(rune), actual_allocLen = 0;
	void* new_strbuff = malloc(init_allocLen);

	switch (self->type) {
	case CHAR_UTF8:
		switch (type) {
		case CHAR_UTF16: { actual_allocLen = std.UTF8.toUtf16(self->data.utf8, self->len, new_strbuff, init_allocLen); break; }
		case CHAR_UTF32: { actual_allocLen = std.UTF8.toUtf32(self->data.utf8, self->len, new_strbuff, init_allocLen); break; }
		case CHAR_ASCII: { actual_allocLen = std.UTF8.toAscii(self->data.utf8, self->len, new_strbuff, init_allocLen); break; }
		default: ERR(STRINGERR_ENCODING, "Invalid target encoding"); return nil;
		}
	case CHAR_UTF16:
		switch (type) {
		case CHAR_UTF8:  { actual_allocLen = std.UTF16.toUtf8 (self->data.utf16, self->len, new_strbuff, init_allocLen); break; }
		case CHAR_UTF32: { actual_allocLen = std.UTF16.toUtf32(self->data.utf16, self->len, new_strbuff, init_allocLen); break; }
		case CHAR_ASCII: { actual_allocLen = std.UTF16.toAscii(self->data.utf16, self->len, new_strbuff, init_allocLen); break; }
		default: ERR(STRINGERR_ENCODING, "Invalid target encoding"); return nil;
		}
	case CHAR_UTF32:
		switch (type) {
		case CHAR_UTF8:  { actual_allocLen = std.UTF32.toUtf8 (self->data.utf32, self->len, new_strbuff, init_allocLen); break; }
		case CHAR_UTF16: { actual_allocLen = std.UTF32.toUtf16(self->data.utf32, self->len, new_strbuff, init_allocLen); break; }
		case CHAR_ASCII: { actual_allocLen = std.UTF32.toAscii(self->data.utf32, self->len, new_strbuff, init_allocLen); break; }
		default: ERR(STRINGERR_ENCODING, "Invalid target encoding"); return nil;
		}
	case CHAR_ASCII:
		// Since ASCII is a valid subset of UTF-8, we can convert to UTF-8 first,
		// and then to UTF-16 or UTF-32. This avoids redundant code.
		switch (type) {
		case CHAR_UTF8:
		case CHAR_UTF16:
		case CHAR_UTF32:
			{ return std.String.Convert(pushString(self->data.utf8, self->len), type); }

		default: ERR(STRINGERR_ENCODING, "Invalid target encoding"); return nil;
		}
	default:
		ERR(STRINGERR_ENCODING, "Invalid source encoding");
		return nil;
	}

	if(init_allocLen != actual_allocLen) 
		new_strbuff = realloc(new_strbuff, actual_allocLen);

	std_String* result = calloc(1, sizeof(std_String));

	*result = (std_String){
		.len 	   = self->len,
		.__type    = std_String_Type,
		.data.utf8 = new_strbuff,
		.type 	   = type,
	};

	result->__private = pntr_shiftcpy(result, sizeof(std_String));
	result->__private->len_bytes = actual_allocLen;

return result;
}

u64 methodimpl(std_String, Scan, va_list args, ...){

	std_String* scanning_string = 
		self->type == CHAR_UTF8 || self->type == CHAR_ASCII ? self : std.String.Convert(self, CHAR_UTF8);

	if(scanning_string == nil){ 
		ERR(ERR_FAIL, "failed to create stream for scanning");
		return 0;
	}


	var strm = push(std_Stream, 
		  std.Stream.Preset.staticBuffer(
		  	scanning_string->data.utf8, 
		  	scanning_string->__private->len_bytes
		  )
	);

	if(strm == nil){
		if(self != scanning_string) del(scanning_string);
		ERR(ERR_FAIL, "failed to create stream for scanning");
		return 0;
	}
	
	if(!args) va_start(args, args);

	u64 scanned_len = std.Type.fmt.print.varArgs(strm, args);

	pop(strm);

	va_end(args);


return scanned_len;
}


DESTROY(std_String){
	
	nonull(self->data.utf8, return err);
	
	if(!priv.inline_alloc)
		free(self->data.utf8);

	if(priv.views){
	    foreach(priv.views, std_String, view)
		del(view);
	}
return OK;
};


PRINT(std_String){

	std_StreamEncoder encoder = 
		self->type == CHAR_UTF8  ? std.UTF8.streamEncoder :
		self->type == CHAR_ASCII ? std.UTF8.streamEncoder :
		self->type == CHAR_UTF16 ? std.UTF16.streamEncoder :
		self->type == CHAR_UTF32 ? std.UTF32.streamEncoder :
		null
	;
	try(){
	    std.Stream.Process
		.start(out)
		.doEncode(encoder, self->data.utf8)
		.end();
	} catch { return 0; }

return self->len;
}

SCAN(std_String){
	nonull(self, return 0);

	u32 len = 0;
	var builder = push(std_StringBuilder, self->type);
	rune c = 0;

	std_StreamDecoder decoder = 
		self->type == CHAR_UTF8  ? std.String.UTF8.Decoder :
		self->type == CHAR_ASCII ? std.String.UTF8.Decoder :
		self->type == CHAR_UTF16 ? std.UTF16.streamDecoder :
		self->type == CHAR_UTF32 ? std.UTF32.streamDecoder :
		null
	;

	if(!decoder) return 0;

	try(){
	    std.Stream.Process
		.start(in)
		.doDecode(decoder, c){
			std.StringBuilder.Append(builder, null, $(c));			
		}
		then.end();
	} catch { return 0; }

	pop(builder);

return len;
}


HASH(std_String){

return hash_bytes(self->data.utf8, priv.len_bytes);
}

len_t vmethodimpl(std_String_Utils, strnlen, void* str, len_t len){

	switch (std.String.Detect(str)) {
	case CHAR_ASCII:{
		return strnlen((char*)str, len);
	break;}
	case CHAR_UTF8:{
		return std.String.UTF8.len(str, len);
	break;}
	case CHAR_UTF16:{
		return std.String.UTF16.len(str, len);
	break;}
	case CHAR_UTF32:{
		return std.String.UTF32.len(str, len);
	break;}
	default:{
		ERR(ERR_INVALID, "invalid string type");
		return 0;
	}
	}	
}

void* vmethodimpl(std_String_Utils, strncpy, void* dest, void* src,  len_t len){

	switch (std.String.Detect(src)) {
	case CHAR_ASCII:{
		return strncpy((char*)dest, src, len);
	break;}
	case CHAR_UTF8:{
		return std.String.UTF8.cpy(dest, src, len);
	break;}
	case CHAR_UTF16:{
		return std.String.UTF16.cpy(dest, src, len);
	break;}
	case CHAR_UTF32:{
		return std.String.UTF32.cpy(dest, src, len);
	break;}
	default:{
		ERR(ERR_INVALID, "invalid string type");
		return null;
	}
	}	

}
bool vmethodimpl(std_String_Utils, strncmp, void* str1, void* str2, len_t len){

	std_char_type 
		str1_type = std.String.Detect(str1), 
		str2_type = std.String.Detect(str2)
	;

	if(str1_type != str2_type) return false;

	switch (str1_type) {
	case CHAR_ASCII:{
		return strncmp(str1, str2, len);
	break;}
	case CHAR_UTF8:{
		return std.String.UTF8.cmp(str1, str2, len);
	break;}
	case CHAR_UTF16:{
		return std.String.UTF16.cmp(str1, str2, len);
	break;}
	case CHAR_UTF32:{
		return std.String.UTF32.cmp(str1, str2, len);
	break;}
	default:{
		ERR(ERR_INVALID, "invalid string type");
		return false;
	}
	}	

}

construct(std_String,
FMT(),
DEF(),
	  
){
	priv.len_bytes = arg.bytes_len;

	self->len = 
		arg.type == CHAR_ASCII ?
	  		arg.bytes_len :
		arg.type == CHAR_UTF8 ?
	  		std.String.UTF8.len(arg.data, args->bytes_len) :
		arg.type == CHAR_UTF16 ?
	  		arg.bytes_len / sizeof(c16) :
		arg.type == CHAR_UTF32 ?
	  		arg.bytes_len / sizeof(c32) :
	  	0
	;

	if(arg.storage == std_StringStorage_Inline){
		self->data.utf8 = 
	  		arg.type == CHAR_UTF8 ?
	  			calloc(self->len + 1, sizeof(char)) : 
	  		arg.type >= CHAR_INVALID ?
	  			null :
	  		calloc(self->len, arg.type)
	    	;

	    	priv.inline_alloc = false;
	}else{
		self->data.utf8 = pntr_shiftcpy(self, sizeof(std_String) + sizeof(std_String_Private));
	    	
		priv.inline_alloc = true;
	}


	memcpy(self->data.utf8, arg.data, priv.len_bytes);
	self->data.utf8[priv.len_bytes] = '\0';
return self;
}




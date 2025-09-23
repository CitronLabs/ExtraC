/**
 * @file strings.c
 * @author blueberry
 * @brief Source file for string manipulation and encoding functions.
 *
 * This file contains the implementations of the methods for the String object,
 * including comparison, copying, concatenation, and encoding conversion.
 */
#include "./strings.h"



/**
 * @details This method compares the contents of two String objects. It first
 * checks for mismatches in length and encoding type. If they match, it iterates
 * through the strings, decoding codepoints from the UTF-8 representation and
 * comparing them.
*/
bool methodimpl(String, Compare,, inst(String) cmp_string){
	
	if(self->len != cmp_string->len 	   || 
	   self->txt.type  != cmp_string->txt.type ||
	   priv->len_bytes != cmp_string->__private->len_bytes
	) 
		return false;

	char
	    * s1 = self->txt.data.utf8,
	    * s2 = cmp_string->txt.data.utf8
	;

	size_t bytes_compared = 0;

	while (*s1 != '\0' && *s2 != '\0' && bytes_compared < priv->len_bytes) {
		// Decode codepoint for s1
		u32 codepoint1 = 0, codepoint2 = 0;
		
		if(decodeUTF8CodePoint(&s1, &codepoint1) != OK &&
		   decodeUTF8CodePoint(&s2, &codepoint2) != OK
		){
			return false;
		}

		
		if (codepoint1 != codepoint2) return false;
		
		bytes_compared++;
	}
	
	if (bytes_compared == priv->len_bytes || (*s1 == '\0' && *s2 == '\0')) {
	    return true;
	}
	
	if (*s1 == '\0') {
	    return false;
	}

return false;
}

/**

 */
inst(String) methodimpl(String,Copy){

	inst(String) res = priv->inline_alloc ?
		malloc(sizeof(data(String)) + sizeof_String_Private) : 
		malloc(sizeof(data(String)) + sizeof_String_Private  + priv->len_bytes)
	;

	res->__private->len_bytes = priv->len_bytes;
	res->__private->inline_alloc = priv->inline_alloc;

	res->len = self->len;
	res->txt = (typeof(res->txt)){
		.type      = res->txt.type,
		.data.utf8 = priv->inline_alloc ? 
			pntr_shiftcpy(res, sizeof(String_Instance) + sizeof(String_Private)) : 
			malloc(priv->len_bytes)
	};

	memcpy(res->txt.data.utf8, self->txt.data.utf8, priv->len_bytes);

return res;
}

inst(String) methodimpl(String, Cat,, bool usingCstr, ...){
	
	inst builder = push(StringBuilder, self->txt.type, self);

	List(void*) catList = pushList(void*, 10);

	va_list args;
	va_start(args, usingCstr);
	void* currString = NULL;

	while ((currString = va_arg(args, void*)) != NULL) {
		List.Append(catList, &currString, 1);
	}

	foreach(catList, void*, str){
	    if(usingCstr){
	    	StringBuilder.Append(builder, pushString((char*)str, maxof(u64)));
	    } else {
	    	StringBuilder.Append(builder, str);
	    }
	}

	inst result = StringBuilder.CreateStr(builder);
	
	pop(catList, builder)

return result;
}

/**
 * @details This function modifies the string in-place by adjusting its internal
 * pointer and length. It correctly handles multi-byte UTF-8 characters by
 * converting the character count to a byte offset before cutting.
 */
inst(String) methodimpl(String, Cut,, bool fromFront, u64 by){
	// Cuts the string in-place by adjusting its pointer and length.
	// No new memory is allocated.
	nonull(self, return NULL);

	// Determine the number of bytes to cut based on the number of characters.
	u64 bytes_to_cut = 0;
	u64 chars_to_cut = by;
	
	// Convert the character count to a byte count.
	// This handles multi-byte UTF-8 characters correctly.
	const char* txt_ptr = self->txt.data.utf8;
	u32 codepoint;
	
	while(bytes_to_cut < priv->len_bytes && chars_to_cut > 0) {
		char* temp_ptr = (char*)txt_ptr + bytes_to_cut;
		u64 old_bytes_to_cut = bytes_to_cut;
		decodeUTF8CodePoint(&temp_ptr, &codepoint);
		bytes_to_cut += (u64)(temp_ptr - (char*)txt_ptr - old_bytes_to_cut);
		chars_to_cut--;
	}
	
	if (bytes_to_cut > priv->len_bytes) {
		// Cannot cut more than the string length.
		ERR(STRINGERR_OUTOFRANGE, "cut amount exceeds string length");
		return NULL;
	}

	if(fromFront){
		// Cut from the front. Shift the pointer and reduce the length.
		self->txt.data.utf8 += bytes_to_cut;
		self->len -= by;
		priv->len_bytes -= bytes_to_cut;
	} else {
		// Cut from the back. Just reduce the length.
		self->len -= by;
		priv->len_bytes -= bytes_to_cut;
	}

return self;}


inst(String) methodimpl(String, Convert,, chartype type){
	nonull(self, return NULL);
	
	// If the target encoding is the same as the source, just return a copy.
	if (self->txt.type == type) {
		return String.Copy(self);
	}
	
	switch (self->txt.type) {
	case CHAR_UTF8:
		switch (type) {
		case CHAR_UTF16: return convert_utf8_to_utf16(self);
		case CHAR_UTF32: return convert_utf8_to_utf32(self);
		case CHAR_ASCII: return convert_utf8_to_ascii(self);
		default: ERR(STRINGERR_ENCODING, "Invalid target encoding"); return NULL;
		}
	case CHAR_UTF16:
		switch (type) {
		case CHAR_UTF8:  return convert_utf16_to_utf8(self);
		case CHAR_UTF32: return convert_utf16_to_utf32(self);
		case CHAR_ASCII: return convert_utf16_to_ascii(self);
		default: ERR(STRINGERR_ENCODING, "Invalid target encoding"); return NULL;
		}
	case CHAR_UTF32:
		switch (type) {
		case CHAR_UTF8:  return convert_utf32_to_utf8(self);
		case CHAR_UTF16: return convert_utf32_to_utf16(self);
		case CHAR_ASCII: return convert_utf32_to_ascii(self);
		default: ERR(STRINGERR_ENCODING, "Invalid target encoding"); return NULL;
		}
	case CHAR_ASCII:
		// Since ASCII is a valid subset of UTF-8, we can convert to UTF-8 first,
		// and then to UTF-16 or UTF-32. This avoids redundant code.
		switch (type) {
		case CHAR_UTF8:
		case CHAR_UTF16:
		case CHAR_UTF32:
			{ return String.Convert(pushString(self->txt.data.utf8, self->len), type); }

		default: ERR(STRINGERR_ENCODING, "Invalid target encoding"); return NULL;
		}
	default:
		ERR(STRINGERR_ENCODING, "Invalid source encoding");
		return NULL;
	}
}

errvt imethodimpl(String,Free){
	self(String);
	
	nonull(self->txt.data.utf8, return err);
	
	if(!priv->inline_alloc)
		free(self->txt.data.utf8);

return OK;
};

u64 methodimpl(String, Scan,, ...){
	va_list args;
	va_start(args, self);

	u64 scanned_len = FormatUtils.ScanVArgs(self, args);

	va_end(args);

return scanned_len;
}

u64 imethodimpl(String, Print,, FormatID* formats, inst(StringBuilder) out){
	self(String);

return StringBuilder.Append(out, self);
}

u64 imethodimpl(String, ScanFor,, FormatID* formats, inst(String) in){
	self(String);

	u32 len = 0;

	foreach(in, rune, c){
		if(iswblank(c)) break;
		else len++;
	}

	switch (in->txt.type) {
	case CHAR_ASCII:
	case CHAR_UTF8:
		*self = *newString(in->txt.data.utf8, len);
	break;
	case CHAR_UTF16:
		*self = *newString(in->txt.data.utf16, len);
	break;
	case CHAR_UTF32:
		*self = *newString(in->txt.data.utf32, len);
	break;
	default:
		return 0;
	}

return len;
}


u32 imethodimpl(String, Hash){
	self(String);

return Map.Hash(self->txt.data.utf8, priv->len_bytes);
}

size_t vmethodimpl(String_Utils, strnlen, TextEncoding text, size_t len){
	switch (text.string.type) {
	case CHAR_ASCII:{
		return strnlen(text.string.data.utf8, len);
	break;}
	case CHAR_UTF8:{
		return utf8_strnlen(text.string.data.utf8, len);
	break;}
	case CHAR_UTF16:{
		return str16nlen(text.string.data.utf16, len);
	break;}
	case CHAR_UTF32:{
		return str32nlen(text.string.data.utf32, len);
	break;}
	default:{
		ERR(ERR_INVALID, "invalid string type");
		return 0;
	}
	}	
}

i64 methodimpl(String, Regex,, inst(String) regex, str_regex_result* result_buffer, size_t buffer_max);

construct(String,
	.Compare = String_Compare,
	.Copy 	 = String_Copy,
	.Regex 	 = String_Regex,
	.Scan 	 = String_Scan,
	.Cut 	 = String_Cut,
	.Cat	 = String_Cat,
	.Convert = String_Convert,
	.Utils	 = {
		.strnlen = String_Utils_strnlen
	},
	.Formatter = {
		.Print = String_Print,
		.Scan  = String_ScanFor
	},
	.__DESTROY = String_Free,
	.__HASH    = String_Hash,
){
	priv->len_bytes = args.bytes_len;

	self->len = 
		args.txt.type == CHAR_ASCII ?
	  		args.bytes_len :
		args.txt.type == CHAR_UTF8 ?
	  		utf8_strnlen(args.txt.data.utf8, args.bytes_len) :
		args.txt.type == CHAR_UTF16 ?
	  		args.bytes_len / sizeof(c16) :
		args.txt.type == CHAR_UTF32 ?
	  		args.bytes_len / sizeof(c32) :
	  	0
	;

	if(!args.inline_alloc){
		self->txt.data.utf8 = 
	  		args.txt.type == CHAR_UTF8 ?
	  			calloc(self->len + 1, sizeof(char)) : 
	  		args.txt.type >= CHAR_INVALID ?
	  			NULL :
	  		calloc(self->len, args.txt.type)
	    	;

	    	priv->inline_alloc = false;
	}else{
		self->txt.data.utf8 = pntr_shiftcpy(self, sizeof(String_Instance) + sizeof(String_Private));
	    	
		priv->inline_alloc = true;
	}

	memcpy(self->txt.data.utf8, args.txt.data.utf8, priv->len_bytes);
	self->txt.data.utf8[priv->len_bytes] = '\0';
return self;
}

u64 Print_VArgs(inst(StringBuilder) builder, va_list args);
u64 Scan_VArgs(inst(String) str, va_list args);

Impl(FormatUtils){
	.FormatVArgs = Print_VArgs,
	.ScanVArgs = Scan_VArgs
};

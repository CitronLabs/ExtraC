#include "../../pkg.h"

import(std)


errvt moduleMethod(std_StringBuilder, Max, u64 max_len){
	nonull(self, return err)
	std.List.Limit(priv.data, max_len);
return OK;
}

u64 moduleMethod(std_StringBuilder, Set, va_list args, ...){
	nonull(self, return 0);

	u64 len = 0;
	
	std.List.Flush(priv.data);

	if(!args) { va_start(args, args); }

	len = std.fmt.print.varArgs(priv.in_stream, args);
	
	va_end(args);

	if(!len){ std.Stream.Flush(priv.in_stream); return 0; }
	
	std.List.Reserve(priv.data, RESERVE_ATLEAST, elements(priv.in_stream));

	// TODO(blueberry): finish this
	
return len;
}
u64 moduleMethod(std_StringBuilder, Append, va_list args, ...){
	
	nonull(self){ return 0; }

	u64 len = 0;

	if(string != null){
		std.List.Append(priv.data, string->txt.data.utf8, string->__private->len_bytes);
		len = string->len;
	}else{

		va_list args;
		va_start(args, string);

		len = FormatUtils.FormatVArgs(self, args);   
	
		va_end(args);
	}
return len;
}
u64 moduleMethod(std_StringBuilder, Prepend, va_list args, ...){
	nonull(self, return 0);
	u64 len = 0;
	

	var builder = push(std_StringBuilder, priv.ctype);

		va_list args;
		va_start(args, string);
	
		len = FormatUtils.FormatVArgs(builder, args);   
		
		va_end(args);

		stringToAdd = std_StringBuilder.GetStr(builder);
		string = &stringToAdd;

	std.List.Insert(priv.data, string->__private->len_bytes, 0, string->txt.data.utf8);
	len = string->len;

return len;
}
u64 moduleMethod(std_StringBuilder, Insert, u64 index, va_list args, ...){
	
	nonull(priv, return 0);
	u64 len = 0;
	
	inst builder = null; 
	data(String) stringToAdd;

	if(string == null){

		builder = push(std_StringBuilder, priv.ctype);

		va_list args;
		va_start(args, string);
	
		len = FormatUtils.FormatVArgs(builder, args);   
		
		va_end(args);

		stringToAdd = std_StringBuilder.GetStr(builder);
		string = &stringToAdd;
	}
		
	std.List.Insert(priv.data, string->__private->len_bytes, index, string->txt.data.utf8);

	priv.len += string->len;

	if(builder) pop(builder);

return string->len;
}

errvt moduleMethod(std_StringBuilder, Clear){
	nonull(self){ return err; }
	std.List.Flush(priv.data);
return OK;
}

String_Instance moduleMethod(std_StringBuilder, GetStr){
	nonull(self, return (String_Instance){0});
	
	priv.interalStrPriv.len_bytes = std.List.Size(priv.data);

	std.List.Append(priv.data, &(u8){0}, 1);

	String_Instance result = {
		.__methods = &String, .__init = true, .__private = &priv.interalStrPriv,
		.len = priv.len,	
		.txt.data.utf8 = std.List.GetPointer(priv.data, 0)
    	};

	std.List.Pop(priv.data, 1);

return result;
}

inst(String) moduleMethod(std_StringBuilder, CreateStr){
	nonull(self){ return nil; }

	u64 len = std.List.Size(priv.data);

return 
	initialize(String, calloc(len + 1, sizeof(u8)),
	    .txt = {
		.type = priv.ctype,
     		.data.utf8 = std.List.GetPointer(priv.data, 0)
	    },
	    .bytes_len = len,
	    .inline_alloc = true
	);
}

u64 moduleIMethod(std_StringBuilder, Print, FormatID* formats, inst(std_StringBuilder) out){
	self(std_StringBuilder);

	std.List.Append(priv.data, "\0", 1);
	
	u64 formated_len = std_StringBuilder.Append(out, null, 
			"(std_StringBuilder){ max_len = ",$(priv.max_len),", str = \"", 
	endstr);
	data(String) internalString = std_StringBuilder.GetStr(self);
	formated_len += std_StringBuilder.Append(out, &internalString);
	
	formated_len += std_StringBuilder.Append(out, s("\" }"));

	std.List.Pop(priv.data, 1);
return formated_len;
}

u64 moduleIMethod(std_StringBuilder, Scan, FormatID* formats, inst(String) in){
	self(std_StringBuilder);

return std_StringBuilder.Append(self, in);
}

errvt moduleIMethod(std_StringBuilder, Free){
	self(std_StringBuilder);
	nonull(self){ return err; }
	del(priv.data);
return OK;
}

construct(std_StringBuilder,
){

	if(arg.type >= CHAR_INVALID)
	  	return nil;

	setpriv(std_StringBuilder){
		.data = newList(u8,  arg.init_str ? 20 : args->init_str->len),
		.max_len = arg.limit == 0 ? UINT64_MAX : args->limit,
	  	.ctype = arg.type,
	};

	std.List.Limit(priv.data, priv.max_len);
	
	if(arg.init_str != null)
		std.List.Append(priv.data, arg.init_str->txt.data.utf8, args->init_str->len);

return self;
}

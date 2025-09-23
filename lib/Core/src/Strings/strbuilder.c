#include "./strings.h"
#include <stdlib.h>
#include <wchar.h>


errvt methodimpl(StringBuilder, Max,, u64 max_len){
	nonull(self, return err)
	List.Limit(priv->data, max_len);
	priv->max_len = max_len;
return OK;
}

u64 methodimpl(StringBuilder, Set,, inst(String) string, ...){

	nonull(priv, return 0;);
	u64 len = 0;
	
	List.Flush(priv->data);

	if(string != NULL){
		List.Append(priv->data, string->txt.data.utf8, string->__private->len_bytes);
		len = string->len;
	}else{
		va_list args;
		va_start(args, string);


		len = FormatUtils.FormatVArgs(self, args);   
		
		va_end(args);
	}
	

return len;
}
u64 methodimpl(StringBuilder, Append,, inst(String) string, ...){
	
	nonull(priv, return 0;);
	u64 len = 0;

	if(string != NULL){
		List.Append(priv->data, string->txt.data.utf8, string->__private->len_bytes);
		len = string->len;
	}else{

		va_list args;
		va_start(args, string);

		len = FormatUtils.FormatVArgs(self, args);   
	
		va_end(args);
	}
return len;
}
u64 methodimpl(StringBuilder, Prepend,, inst(String) string, ...){
	
	nonull(priv, return 0);
	u64 len = 0;
	
	inst builder = NULL; 
	data(String) stringToAdd;

	if(string == NULL){

		builder = push(StringBuilder, priv->ctype);

		va_list args;
		va_start(args, string);
	
		len = FormatUtils.FormatVArgs(builder, args);   
		
		va_end(args);

		stringToAdd = StringBuilder.GetStr(builder);
		string = &stringToAdd;
	}

	List.Insert(priv->data, string->__private->len_bytes, 0, string->txt.data.utf8);
	len = string->len;

return len;
}
u64 methodimpl(StringBuilder, Insert,, u64 index, inst(String) string, ...){
	
	nonull(priv, return 0);
	u64 len = 0;
	
	inst builder = NULL; 
	data(String) stringToAdd;

	if(string == NULL){

		builder = push(StringBuilder, priv->ctype);

		va_list args;
		va_start(args, string);
	
		len = FormatUtils.FormatVArgs(builder, args);   
		
		va_end(args);

		stringToAdd = StringBuilder.GetStr(builder);
		string = &stringToAdd;
	}
		
	List.Insert(priv->data, string->__private->len_bytes, index, string->txt.data.utf8);

	priv->len += string->len;

	if(builder) pop(builder);

return string->len;
}

errvt methodimpl(StringBuilder, Clear){
	nonull(self, return err);
	List.Flush(priv->data);
return OK;
}

String_Instance methodimpl(StringBuilder, GetStr){
	nonull(self, return (String_Instance){0});
	
	priv->interalStrPriv.len_bytes = List.Size(priv->data);

	List.Append(priv->data, &(u8){0}, 1);

	String_Instance result = {
		.__methods = &String, .__init = true, .__private = &priv->interalStrPriv,
		.len = priv->len,	
		.txt.data.utf8 = List.GetPointer(priv->data, 0)
    	};

	List.Pop(priv->data, 1);

return result;
}

inst(String) methodimpl(StringBuilder, CreateStr){
	nonull(self, return NULL);

	u64 len = List.Size(priv->data);

return 
	initialize(String, calloc(len + 1, sizeof(u8)),
	    .txt = {
		.type = priv->ctype,
     		.data.utf8 = List.GetPointer(priv->data, 0)
	    },
	    .bytes_len = len,
	    .inline_alloc = true
	);
}

u64 imethodimpl(StringBuilder, Print,, FormatID* formats, inst(StringBuilder) out){
	self(StringBuilder);

	List.Append(priv->data, "\0", 1);
	
	u64 formated_len = StringBuilder.Append(out, NULL, 
			"(StringBuilder){ max_len = ",$(priv->max_len),", str = \"", 
	endstr);
	data(String) internalString = StringBuilder.GetStr(self);
	formated_len += StringBuilder.Append(out, &internalString);
	
	formated_len += StringBuilder.Append(out, s("\" }"));

	List.Pop(priv->data, 1);
return formated_len;
}

u64 imethodimpl(StringBuilder, Scan,, FormatID* formats, inst(String) in){
	self(StringBuilder);

return StringBuilder.Append(self, in);
}

errvt imethodimpl(StringBuilder, Free){
	self(StringBuilder);
	nonull(self, return err);
	del(priv->data);
return OK;
}

construct(StringBuilder,
	.Append = StringBuilder_Append,
	.Insert = StringBuilder_Insert,
	.Clear = StringBuilder_Clear,
	.CreateStr = StringBuilder_CreateStr,
	.GetStr = StringBuilder_GetStr,
	.Prepend = StringBuilder_Prepend,
	.Max = StringBuilder_Max,
	.Set = StringBuilder_Set,
	.__DESTROY = StringBuilder_Free,
	.Formatter = {
	  	.Print = StringBuilder_Print, 
	  	.Scan = StringBuilder_Scan
	},
){

	if(args.type >= CHAR_INVALID)
	  	return NULL;

	setpriv(StringBuilder){
		.data = newList(u8,  args.init_str == NULL ? 20 : args.init_str->len),
		.max_len = args.limit == 0 ? UINT64_MAX : args.limit,
	  	.ctype = args.type,
	};

	List.Limit(priv->data, priv->max_len);
	
	if(args.init_str != NULL)
		List.Append(priv->data, args.init_str->txt.data.utf8, args.init_str->len);

return self;
}

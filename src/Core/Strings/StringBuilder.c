#include <XC.Core/pkg.c>


errvt moduleMethod(std_StringBuilder,   Max,     u64 max_len);
len_t moduleMethod(std_StringBuilder,   Set,     std_String* string);
len_t moduleMethod(std_StringBuilder,   Append,  std_String* string);
len_t moduleMethod(std_StringBuilder,   Prepend, std_String* string);
len_t moduleMethod(std_StringBuilder,   Insert,  u64 index, std_String* string);
errvt moduleMethod(std_StringBuilder,   Clear);

std_String* moduleMethod(std_StringBuilder, GetStr){
	nonull(self, return (String_Instance){0});
	
	priv.interalStrPriv.len_bytes = std.List.Size(priv.data);

	std.List.Append(priv.data, &(u8){0}, 1);

	std_String result = {
		.__methods = &String, .__init = true, .__private = &priv.interalStrPriv,
		.len = priv.len,	
		.txt.data.utf8 = std.List.GetPointer(priv.data, 0)
    	};

	std.List.Pop(priv.data, 1);

return result;
}

std_String* moduleMethod(std_StringBuilder, CreateStr){
	nonull(self){ return nil; }

	u64 len = std.List.Size(priv.data);

}

construct(std_StringBuilder,
FMT(),
DEF(),
){

	if(arg.type >= CHAR_INVALID)
	  	return nil;

	
	priv.data 		= newList(u8,  arg.init_str ? 20 : args->init_str->len),
	priv.max_len 		= arg.limit == 0 ? maxof(len_t) : args->limit,
	priv.ctype 		= arg.type,
	

	std.List.Limit(priv.data, priv.max_len);
	
	if(arg.init_str != null)
		std.List.Append(priv.data, arg.init_str->txt.data.utf8, args->init_str->len);

return self;
}

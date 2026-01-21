#pragma once

#define __XC_STRINGS__
#include "../pkg.h"
#include "../types.h"

#define package std

Decl(List)

Class(Editor,
INIT(),
FMT(),
private(
	std_List* data;
	std_Stream* in_stream;
	std_char_type ctype;
	rune currentCharacter;
	u64 len;
)
){
	Process(String, std_String,
		fn(strip)(),
		fn(upper)(),
		fn(lower)(),
		fn(append)(std_String*),
	 	fn(prepend)(std_String*),
	 	fn(insert)(len_t, std_String*),
	 	fn(clear)();
		
		std_String
			* fn(getStr),
			* fn(makeStr);

	 	len_t fn(find)(std_String*);

	 	Array(len_t) fn(findAll)(std_String*);

	 	Array(len_t) fn(findSome)(std_String*);
	)
};

#undef package

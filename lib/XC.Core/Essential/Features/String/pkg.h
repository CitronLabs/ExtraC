#pragma once
#define __XC_FEATURE__
#include "../pkg.h"
#include "../Type/pkg.h"
#include "../Arrays/pkg.h"
#include "../Base/pkg.h"
#include "../Stream/pkg.h"


#ifndef __XC_STRING__
#include "utils.h"
#include "types.h"

#include "Encodings/pkg.h"

#define package std

Class(String,
INIT(c8* data; len_t max_len; bool view),
FMT(),
      c8* data; len_t len;
private(
	len_t len_bytes; ArrayList(String) views; bool IsView : 1;
)
){	

	interface(std_String_Encoding) Encoding;

	submodule(Utils,
	   submodule(Str,
		len_t fn(siz)(std_varData str,  len_t maxlen);
		len_t fn(length)(std_varData str,  len_t maxlen, void** end);
		void* fn(cpy)(std_varData dest, std_varData src,  len_t maxlen);
		bool  fn(cmp)(std_varData str1, std_varData str2, len_t maxlen);
	   )
	   submodule(Char,
		bool fn(blank)(rune c);
		bool fn(alpha)(rune c);
		bool fn(digit)(rune c);
	   )
	)

	bool 		method(String, Compare, std_String* string2);
	
	errvt 		method(String, Copy, std_String* to);

	std_String* 	method(String, Cat, Array(std_String*) strings);

	std_String*	method(String, View,          len_t from, len_t to);
	errvt		method(String, ViewShift,     len_t up,   len_t down);
	
	bool 		method(String, IsView);

	errvt		method(String, StreamTo, std_Stream* stream);
};




#undef package
#endif



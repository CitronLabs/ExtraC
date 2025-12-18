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

#define package std

Class(String,
INIT(c8* data; len_t max_len; bool view),
FMT(),
      c8* data; len_t len;
private(
	len_t len_bytes; ArrayList(String) views; bool IsView : 1;
)
){	
	values(Encoding, word,
		UTF8,
		UTF16,
		UTF32,
		ASCII
	)
	submodule(UTF8,
		len_t fn(charSize) (rune codepoint);
		errvt fn(decode) (char** start, rune* codepoint);
		errvt fn(encode) (char* dest,   rune codepoint);
		len_t fn(toUtf16)(c8* in, len_t in_max, c16* dest, len_t dest_max);
		len_t fn(toUtf32)(c8* in, len_t in_max, c32* dest, len_t dest_max);
		len_t fn(toAscii)(c8* in, len_t in_max, c8*  dest, len_t dest_max);
		const std_StreamEncoder Encoder;
		const std_StreamDecoder Decoder;
	)
	submodule(Utils,
	   submodule(Str,
		len_t fn(siz)(void* str,  len_t maxlen);
		len_t fn(len)(void* str,  len_t maxlen, void** end);
		void* fn(cpy)(void* dest, void* src,  len_t maxlen);
		bool  fn(cmp)(void* str1, void* str2, len_t maxlen);
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



#pragma once
#define __XC_STRING__
#include "../pkg.h"

#define package std_String

Interface(Encoding,
	values(Type, word,
		UTF8,
		UTF16,
		UTF32,
		ASCII
	)
	submodule(UTF8,
		len_t fn(charSize) (rune codepoint);
	   	len_t fn(len)(c8* str, len_t max, c8** end);
		errvt fn(decode) (c8** start, rune* codepoint);
		errvt fn(encode) (c8* dest,   rune codepoint);
		len_t fn(toUtf16)(c8* in, len_t in_max, c16* dest, len_t dest_max);
		len_t fn(toUtf32)(c8* in, len_t in_max, c32* dest, len_t dest_max);
		len_t fn(toAscii)(c8* in, len_t in_max, c8*  dest, len_t dest_max);
		const std_StreamEncoder Encoder;
		const std_StreamDecoder Decoder;
	)
	submodule(UTF16,
	   	len_t fn(len)(c16* str, len_t max, c16** end);
		errvt fn(encode) (c16* dest,   rune codepoint);
		len_t fn(toUtf8) (c16* in, len_t in_max, c8* dest, len_t dest_max);
		len_t fn(toUtf32)(c16* in, len_t in_max, c32* dest, len_t dest_max);
		len_t fn(toAscii)(c16* in, len_t in_max, c8*  dest, len_t dest_max);
		const std_StreamEncoder Encoder;
		const std_StreamDecoder Decoder;
	)
	submodule(UTF32,
	   	len_t fn(len)(c32* str, len_t max, c32** end);
		len_t fn(toUtf8) (c32* in, len_t in_max, c8* dest,  len_t dest_max);
		len_t fn(toUtf16)(c32* in, len_t in_max, c16* dest, len_t dest_max);
		len_t fn(toAscii)(c32* in, len_t in_max, c8*  dest, len_t dest_max);
		const std_StreamEncoder Encoder;
		const std_StreamDecoder Decoder;
	)
)

#undef package

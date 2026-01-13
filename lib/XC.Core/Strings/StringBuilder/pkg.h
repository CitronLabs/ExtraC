#pragma once

#define __XC_STRINGS__
#include "../pkg.h"
#include "../types.h"

#define package std

Decl(List)

Class(StringBuilder,
INIT(std_String* init_str; u64 limit),
FMT(),
private(
	std_List* data;
	std_Stream* in_stream;
	std_char_type ctype;
	rune currentCharacter;
	u64 len;
)
){
      	std_StreamEncoder streamEncoder;
      	std_StreamDecoder streamDecoder;

	errvt method(StringBuilder,   Max,     u64 max_len);
	len_t method(StringBuilder,   Set,     std_String* string);
	len_t method(StringBuilder,   Append,  std_String* string);
	len_t method(StringBuilder,   Prepend, std_String* string);
	len_t method(StringBuilder,   Insert,  u64 index, std_String* string);
	errvt method(StringBuilder,   Clear);
	
	std_String* method(StringBuilder, GetStr);
	std_String* method(StringBuilder, CreateStr);

};

#undef package

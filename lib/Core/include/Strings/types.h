#pragma once

#define __XC_STRINGS__
#include "pkg.h"

struct(str_regex_result,
	u64 str_offset;
	u64 len;
);

enum(chartype, CHAR_UTF8 = 0, CHAR_ASCII = 1, CHAR_UTF16 = 2, CHAR_UTF32 = 4, CHAR_INVALID = 5);

struct(char_t,
	chartype type;
	union {
	    char ascii; 
	    char16_t utf16; 
	    char32_t utf32; 
	    wchar_t wide;
	} data;
);

Data(str_t,
INIT(chartype type; void* data),
     	size_t 	 len;
	chartype type;
	union {
	    char* 	utf8; 
	    char16_t* 	utf16; 
	    char32_t* 	utf32; 
	    wchar_t* 	wide;
	} data;
);

#define endstr NULL, NULL

Interface(StringUtils,
	size_t vmethod(len, void* str,  size_t maxlen);
	size_t vmethod(cpy, void* dest, void* src,  size_t maxlen);
	size_t vmethod(cmp, void* str1, void* str2, size_t maxlen);
	size_t vmethod(cat, void* dest, void* src,  size_t maxlen);
);

VTable(UTF8Utils,  interface(StringUtils);)
VTable(UTF16Utils, interface(StringUtils);)
VTable(UTF32Utils, interface(StringUtils);)

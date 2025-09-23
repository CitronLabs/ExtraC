#pragma once

#define __XC_STRINGS__
#include "pkg.h"

Enum(chartype, CHAR_UTF8 = 0, CHAR_ASCII = 1, CHAR_UTF16 = 2, CHAR_UTF32 = 4, CHAR_INVALID = 5);

Type(str_regex_result,
	u64 str_offset;
	u64 len;
);

typedef union{ 
    struct {
	chartype type;
	union {
	    char ascii; 
	    char16_t utf16; 
	    char32_t utf32; 
	    wchar_t wide;
	} data;
    } character;
    struct {
	chartype type;
	union {
	    char* utf8; 
	    char16_t* utf16; 
	    char32_t* utf32; 
	    wchar_t* wide;
	} data;
    } string;
}TextEncoding; 

#define __FORMAT_CODES__
typedef enum FormatID{
	#define FORMAT(domain, default, ...) __VA_ARGS__,
		#include "../config.h"
	#undef FORMAT
}FormatID;

typedef enum Format_Domain{
	#define FORMAT(domain, default, ...) FORMAT_##domain,
		#include "../config.h"
	#undef FORMAT
	FORMAT_DOMAIN_TOP
}Format_Domain;


static const FormatID __default_formats[] = {
	#define FORMAT(domain, default, ...) [FORMAT_##domain] = default,
		#include "../config.h"
	#undef FORMAT
};
#undef __FORMAT_CODES__


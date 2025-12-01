#pragma once

#define __XC_STRINGS__
#include "pkg.h"

#define package std

type(str_regex_result,
	u64 str_offset;
	u64 len;
);

enum(char_type, CHAR_UTF8 = 0, CHAR_ASCII = 1, CHAR_UTF16 = 2, CHAR_UTF32 = 4, CHAR_INVALID = 5);


#define endstr null, NULL

Interface(StringUtils,
);

#undef package

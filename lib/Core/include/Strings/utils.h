#pragma once
#define __XC_STRINGS__
#include "pkg.h"

#define String_From(str_, maxlen_) (&(str_t){NULL, &String, {getCharType(str_), str_}, getStringLen(str_, maxlen_)})
#define String_FromLiteral(str_) (&(str_t){NULL, &String, {getCharType(str_), str_}, sizeof(str_) - 1})

#define String_CutFrntCpy(str, by) String.Cut(&(str_t){NULL, &String, str->txt,  str->len}, CUT_FROMFRNT, by)
#define String_CutBackCpy(str, by) String.Cut(&(str_t){NULL, &String, str->txt, str->len}, CUT_FROMBACK, by)
#define String_CutBack(str, by) String.Cut(str, CUT_FROMFRNT, by)
#define String_CutFrnt(str, by) String.Cut(str, CUT_FROMBACK, by)

#define String_View(str, start, end) String_CutFrntCpy(String_CutBackCpy(str, str->len - end), start)

#define CAT_XCSTRING false
#define CAT_CSTRING  true

#define CUT_FROMBACK false
#define CUT_FROMFRNT true

#define cat(...)     (String.Cat(push(String), CAT_CSTRING, __VA_ARGS__, endstr))
#define catNew(...)  (String.Cat(new(String), CAT_CSTRING, __VA_ARGS__, endstr))
#define xcat(...)    (String.Cat(push(String), CAT_XCSTRING, __VA_ARGS__, endstr))
#define xcatNew(...) (String.Cat(new(String), CAT_XCSTRING, __VA_ARGS__, endstr))

#define switchs(string) inst(String) __string__switch = string; loop(__loop_once__,1)
#define cases(str) if(String.Compare(__string__switch, s(str)))
#define defaults else



#define getCharType(chars) 		\
_Generic((chars),			\
char*: 		CHAR_UTF8,		\
char:  		CHAR_ASCII,		\
char16_t*: 	CHAR_UTF16,		\
char16_t: 	CHAR_UTF16,		\
char32_t*: 	CHAR_UTF32,		\
char32_t: 	CHAR_UTF32,		\
wchar_t*: 	sizeof(wchar_t),	\
wchar_t: 	sizeof(wchar_t),	\
default:	CHAR_INVALID		\
)

#define getCharSize(chars) 		\
_Generic((chars),			\
char*: 		sizeof(char),		\
char:  		sizeof(char),		\
char16_t*: 	sizeof(char16_t),	\
char16_t: 	sizeof(char16_t),	\
char32_t*: 	sizeof(char32_t),	\
char32_t: 	sizeof(char32_t),	\
wchar_t*: 	sizeof(wchar_t),	\
wchar_t: 	sizeof(wchar_t),	\
default:	0			\
)

#define getStringLenBytes(_string, maxlen)		\
_Generic((_string),					\
char*: 		strnlen(generic _string, maxlen),	\
char16_t*: 	String.Utils.strnlen((TextEncoding){	\
			.string = 			\
	 		{CHAR_UTF16, generic _string}	\
	 	}, maxlen) * sizeof(char16_t),		\
char32_t*: 	String.Utils.strnlen((TextEncoding){	\
	 		.string = 			\
	 		{CHAR_UTF32, generic _string}	\
	 	}, maxlen) * sizeof(char32_t),		\
wchar_t*: 	wcsnlen(generic _string, maxlen) 	\
	 		* sizeof(wchar_t) 		\
)

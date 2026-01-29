#pragma once
#include "pkg.h"

#define s(string) push(std_String,			\
	generic string, 				\
	(sizeof(string)) - 1,				\
	true)

#define S(string) new(std_String,			\
	generic string, 				\
	(sizeof(string)) - 1,				\
	true)

#define asString(string, max_len) 			\
	push(std_String, generic string, strsize(string, max_len), true)			


#define cat(...)     (std.String.Cat(push(std_String), arr(__VA_ARGS__)))
#define CAT(...)     (std.String.Cat(new(std_String), arr(__VA_ARGS__)))

#define switchs(string) std_String* __string__switch = string; loop(__loop_once__,1)
#define cases(str) if(std.String.Compare(__string__switch, s(str)))
#define defaults else


#undef strnlen
#define strnlen(_string, maxlen) (std.String.Utils.Str.length(V(_string), maxlen, nil))
#define strsize(_string, maxlen) (std.String.Utils.Str.siz(V(_string), maxlen))
#define strncpy(dest, src, maxlen) (std.String.Utils.Str.cpy(V(dest), V(src), maxlen))
#define strncmp(_string1, _string2, maxlen) (std.String.Utils.Str.cmp(V(_string1), V(_string2), maxlen))

#undef iswblank
#undef iswdigit
#undef iswalpha
#undef iswalnum

#undef isblank
#undef isdigit
#undef isalpha
#undef isalnum

#define iswblank(c) std.String.Utils.Char.blank(c)
#define iswdigit(c) std.String.Utils.Char.digit(c)
#define iswalpha(c) std.String.Utils.Char.alpha(c)
#define iswalnum(c) (std.String.Utils.Char.digit(c) || std.String.Utils.Char.alpha(c))

#define isblank(c) std.String.Utils.Char.blank(c)
#define isdigit(c) std.String.Utils.Char.digit(c)
#define isalpha(c) std.String.Utils.Char.alpha(c)
#define isalnum(c) (std.String.Utils.Char.digit(c) || std.String.Utils.Char.alpha(c))

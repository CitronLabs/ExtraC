#pragma once
#include "pkg.h"

#define cat(...)     (std.String.Cat(push(String), CAT_CSTRING, __VA_ARGS__, endstr))
#define CAT(...)     (std.String.Cat(new(String), CAT_CSTRING, __VA_ARGS__, endstr))

#define switchs(string) std_String* __string__switch = string; loop(__loop_once__,1)
#define cases(str) if(std.String.Compare(__string__switch, s(str)))
#define defaults else


#undef strnlen
#define strnlen(_string, maxlen) (std.String.Utils.Str.len(_string, maxlen))
#define strsize(_string, maxlen) (std.String.Utils.Str.siz(_string, maxlen))


#define iswblank(c) std.String.Utils.Char.blank(c)
#define iswdigit(c) std.String.Utils.Char.digit(c)
#define iswalpha(c) std.String.Utils.Char.alpha(c)
#define iswalnum(c) (std.String.Utils.Char.digit(c) || std.String.Utils.Char.alpha(c))

#define isblank(c) std.String.Utils.Char.blank(c)
#define isdigit(c) std.String.Utils.Char.digit(c)
#define isalpha(c) std.String.Utils.Char.alpha(c)
#define isalnum(c) (std.String.Utils.Char.digit(c) || std.String.Utils.Char.alpha(c))

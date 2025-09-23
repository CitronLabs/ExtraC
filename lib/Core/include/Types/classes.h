#include "../types.h"
#include "../Strings/pkg.h"

Class(Integer,,
__FIELD(union {u64 u64; i64 i64; u32 u32; i32 i32;} asType; u8 sign : 1, longint : 1),
	interface(Formatter);
)

Class(Float,,
__FIELD(union {double f64; float f32;} asType; u8 dbl : 1),
	interface(Formatter);
)

Class(CString,,
__FIELD(typeof((TextEncoding){0}.string) data),
	interface(Formatter);
)

Class(Char,,
__FIELD(typeof((TextEncoding){0}.character) data),
	interface(Formatter);
)

Class(Pointer,, 
__FIELD(void* data),
	interface(Formatter);
)
Class(Boolean,, 
__FIELD(u8 value : 1),
	interface(Formatter);
)

#pragma once
#include "../pkg.h"
#include "../types.h"

#define __FORMAT_CODES__
typedef enum FORMAT_ID{
	#define FORMAT(domain, default, ...) __VA_ARGS__,
		#include "../../config.h"
	#undef FORMAT
}FORMAT_ID;

typedef enum Format_Domain{
	#define FORMAT(domain, default, ...) FORMAT_##domain,
		#include "../../config.h"
	#undef FORMAT
	FORMAT_DOMAIN_TOP
}Format_Domain;


static const FORMAT_ID __default_formats[] = {
	#define FORMAT(domain, default, ...) [FORMAT_##domain] = default,
		#include "../../config.h"
	#undef FORMAT
};
#undef __FORMAT_CODES__

VTable(FormatUtils,
	
	#define FMT ((pntr)1)
	#define F(format_type, format, ...) 				\
		FMT, FORMAT_##format_type, format_type##_##format,	\
		__VA_ARGS__, 						\
		FMT,FORMAT_##format_type, format_type##_##format

	u64 vmethod(FormatVArgs, strbuff_t* out, va_list args);
	u64 vmethod(ScanVArgs,   str_t* in, va_list args);
)

u32 __XCBase_FilePrint(FILE* file, ...);
u32 __XCBase_FileScan(FILE* file, char_t delimiter, ...);

#define $(expr)   NULL, &(asObject((expr))->__ops), generic asObject((expr))

#define endprint NULL, NULL
#define endscan  NULL, NULL

#define __SCAN(class)    u64   imethodimpl(Class, _SCAN,,   FormatID* format,  str_t* in)	
#define __PRINT(class)   u64   imethodimpl(Class, _PRINT,,  FormatID* format,  strbuff_t* out)	

#define print(...) 	 	   __XCBase_FilePrint(stdout, __VA_ARGS__, endprint)
#define fprint(file,...) 	   __XCBase_FilePrint(file,   __VA_ARGS__, endprint)
#define println(...) 	 	   __XCBase_FilePrint(stdout, __VA_ARGS__, "\n" , endprint)
#define scan(delimiter,...) 	   __XCBase_FileScan(stdin, {CHAR_UTF8, .data.wide = delimiter}, __VA_ARGS__, endscan)
#define fscan(file, delimiter,...) __XCBase_FileScan(file,  {CHAR_UTF8, .data.wide = delimiter} ,__VA_ARGS__, endscan)
#define scanln(...) 	 	   __XCBase_FileScan(stdin, (char_t){CHAR_UTF8,"\n"}, __VA_ARGS__, endscan)

Class(Integer,,
FIELD(union {u64 u64; i64 i64; u32 u32; i32 i32;} asType; u8 sign : 1, longint : 1),
)

Class(Float,,
FIELD(union {double f64; float f32;} asType; u8 dbl : 1),
)

Class(CString,,
FIELD(str_t data),
)

Class(Char,,
FIELD(char_t data),
)

Class(Pointer,, 
FIELD(void* data),
)
Class(Boolean,, 
FIELD(u8 value : 1),
)

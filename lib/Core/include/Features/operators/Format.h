#pragma once
#define __XC_FEATURE__
#include "../pkg.h"
#include "../Types/pkg.h"


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

#define $(expr)   NULL, &(asObject((expr))->__ops), generic asObject((expr))


#define print(stream,...) append(stream, __VA_ARGS__) 
#define scan(stream, ...) get(stream, __VA_ARGS__) 

#define println(...) 	 append(Stream.getStdOut(), __VA_ARGS__) 
#define scanln(...) 	 get(Stream.getStdOut(), __VA_ARGS__) 


Class(Integer,,
FIELD(union {u64* u64; i64* i64; u32* u32; i32* i32;} asType; u8 sign : 1, longint : 1),
);

Class(Float,,
FIELD(union {double* f64; float* f32;} asType; u8 dbl : 1),
);


#define CCHAR_8  0
#define CCHAR_16 1
#define CCHAR_32 3

Class(CString,,
FIELD(wchar_t** data; u8 cchar_type : 2),
);

Class(Char,,
FIELD(wchar_t* data; u8 cchar_type : 2),
);

Class(Pointer,, 
FIELD(void** data),
);
Class(Boolean,, 
FIELD(bool* value),
);

#pragma once
#include "../libc.h"

typedef uint64_t u64;

typedef uint32_t u32;

typedef uint16_t u16;


typedef uint8_t u8;

typedef int64_t i64;

typedef int32_t i32;


typedef int16_t i16;

typedef int8_t i8;

typedef struct{ char as_cchar; } c8;

typedef char16_t c16;

typedef char32_t c32;

typedef wchar_t  rune;

typedef char* cstr;


typedef wchar_t* wstr;

#undef bool
typedef _Bool bool;

typedef void* pntr;
#define arry(type) type*
#define pntr_shift(ptr, shift_amt) ptr = ((void*)(&(((uint8_t*)ptr)[shift_amt])))
#define pntr_shiftcpy(ptr, shift_amt) ((void*)(&(((uint8_t*)ptr)[shift_amt])))
#define pntr_asVal(addr) (*(uint64_t*)&addr)


typedef void noFail;
typedef u32 errvt;

typedef struct str_t str_t;						
typedef struct strbuff_t strbuff_t;					
typedef u32 FormatID;

#define struct(name, ...) typedef struct name {__VA_ARGS__} name;
#define structDecl(name)  typedef struct name name;

#define enum(name, ...) typedef enum {__VA_ARGS__} name; 			




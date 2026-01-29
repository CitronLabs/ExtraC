#pragma once
#define __XC_ENV_LIB__
#include "pkg.h"

#include "config.c"

typedef void noFail;

typedef __XC_UINT64_TYPE u64;

typedef __XC_UINT32_TYPE u32;

typedef __XC_UINT16_TYPE u16;


typedef __XC_UINT8_TYPE  u8;

typedef __XC_INT64_TYPE i64;

typedef __XC_INT32_TYPE i32;

typedef __XC_INT16_TYPE i16;

typedef __XC_INT8_TYPE i8;

typedef __XC_C8_TYPE c8;

typedef __XC_C16_TYPE c16;

typedef __XC_C32_TYPE c32;

typedef c8*  strc8;
typedef c16* strc16;
typedef c32* strc32;

typedef __XC_LEN_TYPE len_t;

typedef __XC_WORD_TYPE word;

typedef unsigned __XC_WORD_TYPE uword;

typedef __XC_BYTE_TYPE byte;

typedef unsigned __XC_BYTE_TYPE ubyte;

typedef __XC_RUNE_TYPE rune;

#define var __XC_VAR_TYPE

typedef struct { len_t size; __XC_VARGS_TYPE* data; } vargs;

typedef uword errvt;

#define nil __XC_NIL

typedef void* 			pntr;
typedef const void* 	  	constpntr;
typedef __XC_PNTRVAL_TYPE 	pntrval;


typedef void* devHandle;
typedef void* registerHandle;
typedef void* streamHandle;
typedef void* threadHandle;

typedef int busylock;









#pragma once
#include "../config.c"

#define __XC_UINT64_TYPE unsigned long long       
#define __XC_INT64_TYPE  long long              
#define __XC_UINT32_TYPE unsigned int               
#define __XC_INT32_TYPE  int        
#define __XC_UINT16_TYPE unsigned short    
#define __XC_INT16_TYPE  short      
#define __XC_UINT8_TYPE  unsigned char
#define __XC_INT8_TYPE   signed char

#define __XC_C8_TYPE	char
#define __XC_C16_TYPE	short
#define __XC_C32_TYPE	int

#define __XC_BYTE_TYPE	char
#define __XC_WORD_TYPE	int
#define __XC_LEN_TYPE	unsigned long long
#define __XC_RUNE_TYPE  int

#define __XC_VAR_TYPE  __auto_type

#define __XC_NULL	((void*)0)



#define __XC_VARGS_TYPE   __builtin_va_list
#define __XC_VARGS_START  __builtin_va_start
#define __XC_VARGS_GETARG __builtin_va_arg
#define __XC_VARGS_END    __builtin_va_end


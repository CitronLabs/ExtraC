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

typedef void* var;
#define var __XC_VAR_TYPE

typedef struct { len_t size; __XC_VARGS_TYPE* data; } vargs;

typedef uword errvt;

#undef NULL
#define NULL __XC_NULL

#undef bool
typedef _Bool bool;

typedef void* pntr;
typedef const void* constpntr;

typedef struct __xc_stateData* stateData;


#define streamSettings(...) &(streamSettings){__VA_ARGS__}
typedef struct streamSettings {
	len_t buffer_limit;
} streamSettings;

typedef struct streamInfo {
	streamSettings settings;
	len_t size, time_created, time_modified, currentPos;
	const char* name;
	int attributes;
	bool valid;
} streamInfo;

typedef struct registerInfo {
	len_t size, time_created, time_modified;
	const char* name;
	int attributes;
	bool valid;
} registerInfo;

#define streamHandle_Invalid ((streamHandle)0)
typedef struct __xc_stream_type{}* streamHandle;

typedef struct timeSpec {
    	u64 seconds;
    	u64 nanoseconds;
} timeSpec;

typedef struct __xc_register_type{}* registerHandle;
typedef struct __xc_device_type{}*   devHandle;

typedef union XC_Sys_Arch_Flags{ 
	struct{
	    word 
	 	carry 	 : 1,
	     	zero  	 : 1,
	     	overflow : 1,
	     	sign 	 : 1;
	} flags;
	word allFlags;

}XC_Sys_Arch_Flags;


typedef struct XC_Locale_Data {

	const char * Identifier;

	namespace(Time,
	    const char
	    	* day_names[7],
	    	* short_day_names[7],
	    
	    	* month_names[7],
	    	* short_month_names[7]
	    ;

	    byte first_day_in_week;
	)
	namespace(Currency,
	    const char 
	   	* currency_symbol,
	    	* int_curr_symbol,
    	     	* mon_decimal_point,        
    	     	* mon_thousands_sep,        
    	    	* positive_sign,            
    	    	* negative_sign,            
    	    	* mon_grouping;          
    	    int 
		frac_digits,                
    	    	int_frac_digits,            

    	    	pos_cs_precedes,              
    	    	pos_sep_by_space,             
    	    	pos_sign_posn,                

    	    	neg_cs_precedes,              
    	    	neg_sep_by_space,             
    	    	neg_sign_posn;                
	)
	namespace(Measure,
	// System of Measurement
	    word measurement_system;           // The primary system for length, weight, and volume
	    
	// Temperature System
	    word temperature_system;
	)

	
} XC_Locale_Data;

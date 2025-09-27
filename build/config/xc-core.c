/*
ExtraC CORE Configuration File
*/
/**
* @file xc-core.c
* @brief ExtraC's core configuration file
* @author Blueberry
* @version 0.1.0
*/

#if defined (			__ERROR_CODES__				)
/*---------------------------------------------------------------------------*/
	ERR_NONE, ERR_INVALID, ERR_NULLPTR, ERR_INITFAIL, 
	ERR_NOTIMPLEM, ERR_FAIL, ERR_BUSY,

	ERR_SEGFAULT, ERR_ABORT,

	DATAERR_MEMALLOC, DATAERR_SIZETOOLARGE, 	
	DATAERR_OUTOFRANGE,DATAERR_LIMIT, 		
	DATAERR_EMPTY, DATAERR_DSN,		

	MEMERR_OVERFLOW, MEMERR_INITFAIL, MEMERR_LEAK, MEMERR_INVALIDSIZE,	

	STRINGERR_REGEX, STRINGERR_FORMAT, STRINGERR_ENCODING, STRINGERR_OUTOFRANGE,

	#include "./xc-os.c"
	#include "./xc-resource.c"
	#include "./xc-routines.c"

#elif defined( 			__FORMAT_CODES__			)
/*---------------------------------------------------------------------------*/
#ifndef FORMAT
#define FORMAT(domain, default, ...)
#endif	
	       	FORMAT(NUM, NUM_REG,
	     		NUM_REG, NUM_HEX, NUM_FLOAT, NUM_BIN)

		FORMAT(STRING, STRING_REG,
			STRING_REG, STRING_WORD, STRING_NUM)
		
		FORMAT(DATA, DATA_DSN,
			DATA_DSN, DATA_DEBUG)
	
	#include "./xc-os.c"
	#include "./xc-resource.c"


#undef FORMAT
#elif defined( 			__TYPE_OPERATORS__			)
/*---------------------------------------------------------------------------*/
#include "./xc-resource.c"							
#ifndef OPERATOR
#define OPERATOR(class, name, returnval, ...)
#endif	

	#define OPERATOR_FUNCS(Class)							\
	    OPERATOR(Class, Destroy, errvt);						\
	    OPERATOR(Class, Hash,    u32);						\
	    OPERATOR(Class, Append,  u64,,     pntr data[]);				\
	    OPERATOR(Class, Set,     errvt,,   void* value);				\
	    OPERATOR(Class, Copy,    errvt,,   void* where);				\
	    OPERATOR(Class, Iter,    void*,,   u64 index);				\
	    OPERATOR(Class, Size,    size_t,,  bool elements);				\
	    OPERATOR(Class, Scan,    u64,,     FormatID* format,  str_t* in);		\
	    OPERATOR(Class, Print,   u64,,     FormatID* format,  strbuff_t* out);	\
	    RESOURCE_METHODS(Class)							
#else
/*---------------------------------------------------------------------------*/
#ifndef XC_CORE_CONFIG
#define XC_CORE_CONFIG
#include "__config_start.h"
#include "xc-build.h"
/** 
 *  \ingroup ExtraC 
 *  \defgroup xc-core-config Core
 */

#define SIMD_MATH 1
#define SIMD_BITS 2
#define SIMD_CMP  4
#define SIMD_MOVE 8


#define __HeaderOnly			false

#define __Debug 			true

#define __HijackMalloc 			true

#define __stdLoggerName			"Log"

#define __Simd_Use			(SIMD_MATH | SIMD_BITS | SIMD_CMP | SIMD_MOVE)


define(XC_DataStructs){
	value maxSize; setting autoGrow;
}; 

global config(XC_DataStructs) 
__List = { 
	.maxSize 			= INT64_MAX,
	.autoGrow			= true
},
__Queue = { 
	.maxSize 			= INT64_MAX,
	.autoGrow			= true,
},
__Stack = { 
	.maxSize 			= INT64_MAX,
	.autoGrow			= true,
},
__Map = { 
	.maxSize 			= INT64_MAX,
	.autoGrow			= true
};


#include "__config_end.h"
#endif
#endif


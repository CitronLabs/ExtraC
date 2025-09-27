#pragma once

#define __XC_MATH__
#include "../pkg.h"
#include "../types.h"
#include "../../config.h"

#if __Simd_Use

    #define usingSIMD (true)
    
    	#if isSet(__Simd_DefFunctions, SIMD_MATH)
    		#include "SIMD/Arithmetic.h"
    	#endif
    	#if isSet(__Simd_DefFunctions, SIMD_MATH)
    		#include "SIMD/Bitwise.h"
    	#endif
    	#if isSet(__Simd_DefFunctions, SIMD_MATH)
    		#include "SIMD/Comparision.h"
    	#endif
    	#if isSet(__Simd_DefFunctions, SIMD_MATH)
    		#include "SIMD/Move.h"
    	#endif


#define pushSIMD(type, len)  				\
	initialize(SIMD,				\
		alloca(sizeof(SIMD_Instance)),		\
		(sizeof(type) * len), sizeof(type)	\
	)


#define newSIMD(type, len)  				\
	initialize(SIMD,				\
		malloc(sizeof(SIMD_Instance)),		\
		(sizeof(type) * len), sizeof(type)	\
	)

#define MD(first, ...)  					\
	initialize(SIMD,					\
		malloc(sizeof(SIMD_Instance)),			\
		(sizeof((typeof(first)[]){__VA_ARGS__})),	\
	     		sizeof(typeof(first)), 			\
		(typeof(first)[]){first, __VA_ARGS__}		\
	)

#define md(first, ...)  					\
	initialize(SIMD,					\
		alloca(sizeof(SIMD_Instance)),			\
		(sizeof((typeof(first)[]){__VA_ARGS__})),	\
	     		sizeof(typeof(first)), 			\
		(typeof(first)[]){first, __VA_ARGS__}		\
	)

Class(SIMD,
INIT(u16 bitwidth; u16 typesize; void* data),
FIELD(u16 bitwidth, typesize; void* data),
	u32 maxBitWidth;
       	void method(SIMD, Add,,   inst(SIMD) b, inst(SIMD) result);	
       	void method(SIMD, Sub,,   inst(SIMD) b, inst(SIMD) result);	
       	void method(SIMD, Mul,,   inst(SIMD) b, inst(SIMD) result);	
       	void method(SIMD, Div,,   inst(SIMD) b, inst(SIMD) result);	
       	void method(SIMD, And,,   inst(SIMD) b, inst(SIMD) result);	
       	void method(SIMD, Or,,    inst(SIMD) b, inst(SIMD) result);	
       	void method(SIMD, Xor,,   inst(SIMD) b, inst(SIMD) result);	
       	void method(SIMD, Not,,   inst(SIMD) result);	
       	void method(SIMD, Cmp,,   inst(SIMD) b, inst(SIMD) result, numEquality cmpFlags);	
       	void method(SIMD, Move,,  inst(SIMD) output);	
       	void method(SIMD, Store,, void* output);	
       	void method(SIMD, Set,,   void* output);
)
#else
	#define usingSIMD (false)
#endif





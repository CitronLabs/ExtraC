#include "../config.h"
#if __Simd_Use

    #define usingSIMD (true)
    
    	#if isSet(__Simd_Use, SIMD_MATH)
    		#include "SIMD/Arithmetic.h"
    	#endif
    	#if isSet(__Simd_Use, SIMD_BITS)
    		#include "SIMD/Bitwise.h"
    	#endif
    	#if isSet(__Simd_Use, SIMD_CMP)
    		#include "SIMD/Comparision.h"
    	#endif
    	#if isSet(__Simd_Use, SIMD_MOVE)
    		#include "SIMD/Move.h"
    	#endif
#else
	#define usingSIMD (false)
#endif

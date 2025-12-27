#pragma once
#include "../types.h"

Interface(__XC_VEC_MATH,
    	// 'element_size' MUST be a power of 2 (1, 2, 4, 8) to define the unit of arithmetic.
    	void* fn(add)(void* dest, const void* src1, const void* src2, len_t total_size, byte element_size);
    
    	// Element-wise subtraction of two buffers. *dest = *src1 - *src2.
    	void* fn(sub)(void* dest, const void* src1, const void* src2, len_t total_size, byte element_size);

    	// Element-wise multiplication of two buffers. *dest = *src1 * *src2.
    	void* fn(mul)(void* dest, const void* src1, const void* src2, len_t total_size, byte element_size);
    	
	// Element-wise division of two buffers. *dest = *src1 / *src2.
    	void* fn(div)(void* dest, const void* src1, const void* src2, len_t total_size, byte element_size);



)

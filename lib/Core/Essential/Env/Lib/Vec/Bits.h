#pragma once
#include "../types.h"


Interface(__XC_VEC_BITS,
    	// Element-wise bitwise AND of two buffers. *dest = *src1 & *src2.
    	void* fn(and)(void* dest, const void* src1, const void* src2, len_t total_size, byte element_size);

    	// Element-wise bitwise OR of two buffers. *dest = *src1 | *src2.
    	void* fn(or)(void* dest, const void* src1, const void* src2, len_t total_size,  byte element_size);

    	// Element-wise bitwise XOR of two buffers. *dest = *src1 ^ *src2.
    	void* fn(xor)(void* dest, const void* src1, const void* src2, len_t total_size, byte element_size);
)

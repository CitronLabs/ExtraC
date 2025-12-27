#pragma once
#include "../types.h"

Interface(__XC_VEC_MEM,
    	void* fn(cpy)(void* dest, const void* src, len_t size);
    
    	word fn(cmp)(const void* buffer1, const void* buffer2, len_t size);
    
    	void* fn(setVal)(void* dest, word value, len_t size);

    	// 'pattern_size' MUST be a power of 2 (e.g., 4, 8, 16) for optimal vectorization.
    	void* fn(fillPattern)(void* dest, const void* pattern, len_t pattern_size, len_t total_size);

)

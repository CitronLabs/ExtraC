#pragma once

#define __XC_ALLOC__
#include "../pkg.h"
#include "../types.h"

#define package std

/**
@class Arena
@implements Allocator
@brief Basic arena allocator implementation

@details The Buffer class is wrapper around a raw C array which
provides utilities for growing, casting, and allocating slots within
the buffer

@subsection Constructor
@brief Initializes a Arena object
@param init_size 	the initial number of bytes in the arena
@param isStatic 	if the buffer is able to grow 
*/

Class(Arena,
INIT(u64 init_size; bool isStatic),
FMT(),
private(
	len_t max_size, alloc_size, current_size;
	ArrayList(Buffer) arena_buffers;
	bool isStatic;
)
){
      	u64 method(Arena, Size);
      	void* method(Arena, Alloc, u64 num_bytes, std_ErrorPosition errorPos);
      	errvt method(Arena, Reserve, u64 num_bytes);
      	errvt method(Arena, Grow, u64 add_num_bytes);
      };

#undef package

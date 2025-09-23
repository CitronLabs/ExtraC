#pragma once

#define __XC_ALLOC__
#include "../pkg.h"
#include "../types.h"
#include "../interfaces/Allocator.h"

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
__INIT(u64 init_size; bool isStatic),
__FIELD(),
	interface(Allocator);
      	u64 method(Arena, Size);
      	void* method(Arena, Alloc,, u64 num_bytes);
      	errvt method(Arena, Reserve,, u64 num_bytes);
      	errvt method(Arena, Grow,, u64 add_num_bytes);
);

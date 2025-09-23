#pragma once

#define __XC_ALLOC__
#include "../pkg.h"
#include "../types.h"
#include "../interfaces/Allocator.h"


/**
@class CMalloc
@implements Allocator
@brief A wrapper around the standard C allocation functions

@details This class is wrapper around the standard C allocation functions
but with the added utilities of being able to track the total number of 
bytes allocated which could be useful in tracking memory leaks.

there is also an optional callback for when there is an attempt to allocate 
more than max_alloc

@subsection Constructor
@brief Initializes a CMalloc object
@param max_alloc	the maxium number of bytes that can be allocted with this object
*/
Class(CMalloc,
__INIT(u64 max_alloc),
__FIELD(
	bool(*overflow_handler)(inst(CMalloc), u64 overflowed_datasize);
),
	interface(Allocator);
	void* method(CMalloc, AlignNew,, size_t alignment, size_t size);
)


extern inst(CMalloc) c_malloc;

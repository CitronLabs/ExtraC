#pragma once

#define __XC_ALLOC__
#include "../pkg.h"
#include "../types.h"
#include "../interfaces/Allocator.h"

/**
@class Pool
@implements Allocator
@brief Basic pool allocator implementation

@details A basic dynamic pool allocator implementation allowing for methods 
like reserving and growing 

Although at face value this class is very similar to List class its primary 
distinction is that any pointer to an allocated member remains valid for the 
lifetime of the Pool object.

@subsection Constructor
@brief Initializes a Pool object
@param init_size 	the initial number of elements in the arena
@param member_size 	the size of a single element in the arena
@param isStatic 	if the Pool is able to grow beyond the orginal init size
*/
Class(Pool, 
INIT(u64 member_size, init_size, limit; bool isStatic),
FIELD(),
	#define Pool(type) inst(Pool)

	interface(Allocator);
      	u64   method(Pool, Size);
      	void* method(Pool, Alloc,, u64 num);
      	errvt method(Pool, Return,, void* instance);
      	errvt method(Pool, Reserve,, u64 num_members);
      	errvt method(Pool, Grow,, u64 add_num_members);
)

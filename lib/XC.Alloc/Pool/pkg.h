#pragma once

#define __XC_ALLOC__
#include "../pkg.h"
#include "../types.h"

#define package std

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
INIT(std_typeData type; len_t init_size, limit; bool isStatic; intf(std_Allocator) parentAllocator),
FMT(),
private(
	std_typeData type;
	len_t max_size, alloc_size, current_size, num_in_use;
	ArrayList(void*) free_slots;
	ArrayList(Buffer) pool_buffers;
	bool isStatic;
)
){
	errvt	method(Pool, ForceDestroy);
      	void* 	method(Pool, Alloc, u64 num, std_CodePos errorPos);
      	errvt 	method(Pool, Return, void* instance, std_CodePos errorPos);
      	errvt 	method(Pool, Reserve, u64 num_members);
      	errvt 	method(Pool, Grow, u64 add_num_members);
};

#define Pool(type) std_Pool*

#undef package

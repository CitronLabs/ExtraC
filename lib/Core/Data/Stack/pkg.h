#pragma once

#define __XC_DATA__ 
#include "../pkg.h"
#include "../types.h"

#define package std

/**
@class Stack
@brief a dynamic stack of any type of arbitrary size
@details This class represents a dynamic stack which can be 
used wherever an unknown amount of items must be put into a stack


@subsection Constructor
@brief Initializes a Buffer object
@param init_size	the initial number of elements in the list
@param type_size	the size of a single element in the list
@param dsn_type 	the dsn_type of the element's type, this is used for DSN formating
@param literal		a pointer to an array of the same size, this data will be copied into the internal buffer


@subsection DSN
@details This is one of the basic data type in the DSN format anotated by the >>[...] syntax

if a stack gets overidden we compare the counts from each stack
if the orignal reference count is larger than the overide stack's count then the remaining
elements from the orignal reference are appended to overide stack
*/

Class(Stack,
INIT(u64 init_size; std_typeData type; void* literal),
FMT(bool debug),
private(
	void* start;
	u64 allocednum, top, limit;
	std_typeData type;
)){
	
	errvt  		method(Stack, Index, bool write, u64 index, void* data);
	void*  		method(Stack, ToPointer);
	errvt  		method(Stack, Limit, u64 limit);
	errvt  		method(Stack, Reserve, bool exact, u64 amount);
	errvt  		method(Stack, Push, void* item, u64 num);
	errvt  		method(Stack, Pop, void* out, u64 num);
	void*  		method(Stack, FreeToPointer);
	u64    		method(Stack, Count);
	bool   		method(Stack, Check);
	noFail 		method(Stack, Flush);
	std_typeData	method(Stack, GetType);
};

#undef package

/**
@def newStack(type, ...)
@brief allocates a static buffer object on the stack 
*/
#define newStack(_type, size) new(std_Stack, size, typeDataOf(_type))
/**
@def pushStack(type, size)
@brief allocates a static buffer object on the stack 
*/
#define pushStack(_type, size) push(std_Stack, size, typeDataOf(_type))

/**
@def l_s(first, ...)
@brief allocates a list object literal on the stack 
@details the type is infered using typeof() from the first parameter, then the others are assumed to 
be of the same type
*/


#define l_s(first, ...) 				\
push(Stack, 						\
	lenof((typeof(first)[]){first, __VA_ARGS__}),	\
	typeDataFrom(asObject(first)), 			\
	(typeof(first)[]){first, __VA_ARGS__}		\
)
/**
@def L_S(first, ...)
@brief allocates a list object literal on the heap 
@details the type is infered using typeof() from the first parameter, then the others are assumed to 
be of the same type
*/
#define L_S(first, ...) 				\
new(Stack, 						\
	lenof((typeof(first)[]){first, __VA_ARGS__}),	\
	typeDataFrom(asObject(first)), 			\
	(typeof(first)[]){first, __VA_ARGS__}		\
)

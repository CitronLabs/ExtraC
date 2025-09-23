#pragma once

#define __XC_DATA__ 
#include "../pkg.h"
#include "../types.h"


/**
@class Stack
@implements Formatter
@implements IterableList
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
__INIT(u64 init_size; u64 type_size; DSN_fieldType dsn_type; void* literal;),
__FIELD(),
	
	interface(Formatter);
	interface(IterableList);	

	errvt method(Stack,Index,, bool write, u64 index, void* data);
	void* method(Stack,ToPointer);
	errvt method(Stack,Limit,, u64 limit);
	errvt method(Stack,Reserve,, bool exact, u64 amount);
	errvt method(Stack,Push,, void* item, u64 num);
	errvt method(Stack,Pop,, void* out, u64 num);
	void* method(Stack,FreeToPointer);
	u64   method(Stack,Count);
	bool  method(Stack,Check);
)


/**
@def newStack(type, ...)
@brief allocates a static buffer object on the stack 
*/
#define newStack(type, size) new(Stack, 		\
		size,					\
		sizeof(type),	 			\
		getDSN_Type((type){0}),			\
		NULL)
/**
@def pushStack(type, size)
@brief allocates a static buffer object on the stack 
*/
#define pushStack(type, size) push(Stack, 		\
		size,					\
		sizeof(type),	 			\
		getDSN_Type((type){0}),			\
		NULL)
/**
@def l_s(first, ...)
@brief allocates a list object literal on the stack 
@details the type is infered using typeof() from the first parameter, then the others are assumed to 
be of the same type
*/
#define l_s(first, ...) push(Stack, sizeof((typeof(first)[]){first, __VA_ARGS__}) \
				 / sizeof(typeof(first)), 			  \
			   	 sizeof(typeof(first)), 			  \
				 getDSN_Type((typeof(first)){0}),		  \
				 (typeof(first)[]){first, __VA_ARGS__}		  \
			)
/**
@def L_S(first, ...)
@brief allocates a list object literal on the heap 
@details the type is infered using typeof() from the first parameter, then the others are assumed to 
be of the same type
*/
#define L_S(first, ...) new(Stack, sizeof((typeof(first)[]){first, __VA_ARGS__}) \
				 / sizeof(typeof(first)), 			 \
			   	 sizeof(typeof(first)), 			 \
				 getDSN_Type((typeof(first)){0}),		 \
				 (typeof(first)[]){first, __VA_ARGS__}		 \
			)

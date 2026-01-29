#pragma once

#define __XC_ALLOC__
#include "../pkg.h"
#include "../types.h"
#include "../utils.h"

#define package std

/**
Buffer
@implements Allocator
@brief A fixed size array able to store any type of arbitrary size

@details This class is wrapper around a raw C array which
provides utilities for growing, casting, and allocating slots within
the buffer

This class is useful for passing as a parameter within a function,
since it inhertedly maintains an expected size for the buffer it 
can save you an extra size parameter for pointers

@subsection Constructor
@brief Initializes a Buffer object
@param size 		the number of elements in the buffer
@param type_size	the size of a single element in the buffer
@param isStatic 	if the buffer is able to grow 
@param initData		a pointer to an array of the same size, this data will be copied into the internal buffer
*/

Class(Buffer,
INIT(u64 size; Type_t type; bool isStatic; void* initData; intf(std_Allocator) parentAllocator),
FMT(),
private(
	std_typeData type;
	len_t size, alloced_size;
	pntr data; bool isStatic;
)){
	interface(std_Allocator) Allocator;

/**
@return a pointer to the internal buffer
*/
	pntr  		method(Buffer, getPointer);
/**
@return the number of bytes the buffer can hold
*/
	u64   		method(Buffer, getMaxSize);
/**
@return the size of one item in the buffer
*/
	std_typeData 	method(Buffer, getType);
/**
@return the number of items the buffer can hold
*/
	u64   		method(Buffer, getMaxItems);

/**
@brief reallocates the buffer to a specified size
@param num the number of items to resize the buffer to
@return ERR_NONE if 
*/
      	errvt 		method(Buffer, resize, u64 num, std_CodePos errorPos);

/**
@brief casts the buffer's type to  another size
@param type_size the size of the new type
@return ERR_NONE if 
*/
      	errvt 		method(Buffer, cast, Type_t type);

/**
@brief create a static buffer view from a pointer without copying data
@param start the pointer to start of the data
@param type_size the size of the buffer view's type
@param len the number of items in the buffer view
@return ERR_NONE if 
*/
      	std_Buffer* 	method(Buffer, fromView, void* start, Type_t type, u64 len);
};

#undef package

/**
@def newBuffer(type, size)
@brief allocates a static buffer object on the heap
*/
	#define newBuffer(type, size)		 						\
		initialize(Buffer, malloc(							\
			sizeof(Buffer) + sizeof_Buffer_Private + (sizeof(type) * size)),	\
	     		size, sizeof(type), true						\
	     	)
	
/**
@def pushBuffer(type, size)
@brief allocates a static buffer object on the stack 
*/
	#define pushBuffer(type, size)		 						\
		initialize(Buffer, alloca(							\
			sizeof(Buffer) + sizeof_Buffer_Private + (sizeof(type) * size)),	\
	     		size, sizeof(type), true						\
	     	)
	
/**
@def b(first, ...)
@brief allocates a static buffer object litteral on the stack 
@details the type is infered using typeof() from the first parameter, then the others are assumed to 
be of the same type
*/
	#define b(first, ...) 							\
		create(std_Buffer, 							\
		    (memcpy(							\
			malloc(							\
				sizeof(std_Buffer) + 				\
				sizeof((typeof(first)[]){first, __VA_ARGS__}))	\
			+ sizeof(std_Buffer),					\
		        (typeof(first)[]){first, __VA_ARGS__}, 			\
        	        sizeof((typeof(first)[]){first, __VA_ARGS__})) 		\
       		     - sizeof(std_Buffer)),						\
		sizeof((typeof(first)[]){first, __VA_ARGS__}), 			\
		sizeof(typeof(first)),						\
		true, nil 							\
	     	)
/**
@def B(first, ...)
@brief allocates a static buffer object litteral on the heap 
@details the type is infered using typeof() from the first parameter, then the others are assumed to 
be of the same type
*/
	#define B(first, ...) 							\
		create(std_Buffer, 							\
		    (memcpy(							\
			malloc(							\
				sizeof(std_Buffer) + 				\
				sizeof((typeof(first)[]){first, __VA_ARGS__}))	\
			+ sizeof(std_Buffer),					\
		        (typeof(first)[]){first, __VA_ARGS__}, 			\
        	        sizeof((typeof(first)[]){first, __VA_ARGS__})) 		\
       		     - sizeof(std_Buffer)),						\
		sizeof((typeof(first)[]){first, __VA_ARGS__}), 			\
		sizeof(typeof(first)),						\
		true, nil 							\
	     	)


	#define pushBufferView(type, len, pntr) \
			Buffer.fromView(alloca(sizeof(std_Buffer)), pntr, sizeof(type), len)

	#define newBufferView(type, len, pntr) \
			Buffer.fromView(malloc(sizeof(std_Buffer)), pntr, sizeof(type), len)






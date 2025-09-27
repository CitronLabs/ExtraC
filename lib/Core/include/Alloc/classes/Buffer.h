#pragma once

#define __XC_ALLOC__
#include "../pkg.h"
#include "../types.h"
#include "../utils.h"
#include "../interfaces/Allocator.h"

#undef b
#undef B

/**
@class Buffer
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
INIT(u64 size; u16 typeSize; bool isStatic; void* initData),
FIELD(),

	interface(Allocator);

/**
@return **true** if the buffer is at its allocation max else it returns **false**
*/
	bool  method(Buffer, isMaxed);
/**
@return a pointer to the internal buffer
*/
	pntr  method(Buffer, getPointer);
/**
@return the number of bytes the buffer can hold
*/
	u64   method(Buffer, getTotalSize);
/**
@return the size of one item in the buffer
*/
	u64   method(Buffer, getTypeSize);
/**
@return the number of items the buffer can hold
*/
	u64   method(Buffer, getItemNum);
/**
@brief reallocates the buffer to a specified size
@param num the number of items to resize the buffer to
@return ERR_NONE if 
*/
      	errvt method(Buffer, resize,, u64 num);
/**
@brief casts the buffer's type to  another size
@param type_size the size of the new type
@return ERR_NONE if 
*/
      	errvt method(Buffer, cast,, u64 type_size);

/**
@brief create a buffer view from a pointer
@param start the pointer to start of the data
@param type_size the size of the buffer view's type
@param len the number of items in the buffer view
@return ERR_NONE if 
*/
      	inst(Buffer) method(Buffer, fromView,, void* start, u64 type_size, u64 len);
)

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
		initialize(Buffer, 						\
		    (memcpy(							\
			alloca(							\
				sizeof(Buffer) + sizeof_Buffer_Private + 	\
				sizeof((typeof(first)[]){first, __VA_ARGS__}))	\
			+ sizeof(Buffer) + sizeof_Buffer_Private,		\
		        (typeof(first)[]){first, __VA_ARGS__}, 			\
        	        sizeof((typeof(first)[]){first, __VA_ARGS__})) 		\
       		     - sizeof(Buffer) + sizeof_Buffer_Private),			\
		sizeof((typeof(first)[]){first, __VA_ARGS__}), 			\
		sizeof(typeof(first)),						\
		true, NULL 							\
	     	)
/**
@def B(first, ...)
@brief allocates a static buffer object litteral on the heap 
@details the type is infered using typeof() from the first parameter, then the others are assumed to 
be of the same type
*/
	#define B(first, ...) 							\
		initialize(Buffer, 						\
		    (memcpy(							\
			malloc(							\
				sizeof(Buffer) + sizeof_Buffer_Private + 	\
				sizeof((typeof(first)[]){first, __VA_ARGS__}))	\
			+ sizeof(Buffer) + sizeof_Buffer_Private,		\
		        (typeof(first)[]){first, __VA_ARGS__}, 			\
        	        sizeof((typeof(first)[]){first, __VA_ARGS__})) 		\
       		     - sizeof(Buffer) + sizeof_Buffer_Private),			\
		sizeof((typeof(first)[]){first, __VA_ARGS__}), 			\
		sizeof(typeof(first)),						\
		true, NULL 							\
	     	)


	#define pushBufferView(type, len, pntr) \
			Buffer.fromView(alloca(sizeof(Buffer) + sizeof_Buffer_Private), pntr, sizeof(type), len)

	#define newBufferView(type, len, pntr) \
			Buffer.fromView(malloc(sizeof(Buffer) + sizeof_Buffer_Private), pntr, sizeof(type), len)






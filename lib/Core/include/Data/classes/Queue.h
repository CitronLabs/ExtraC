#pragma once

#define __XC_DATA__ 
#include "../pkg.h"
#include "../types.h"

/**
@class Queue
@implements Formatter
@implements IterableList
@brief a dynamic queue of any type of arbitrary size
@details This class represents a dynamic queue which can be 
used wherever an unknown amount of items must be put into a queue


@subsection Constructor
@brief Initializes a Buffer object
@param init_size	the initial number of elements in the list
@param type_size	the size of a single element in the list
@param dsn_type 	the dsn_type of the element's type, this is used for DSN formating
@param literal		a pointer to an array of the same size, this data will be copied into the internal buffer


@subsection DSN
@details This is one of the basic data type in the DSN format anotated by the <<[...] syntax

if a queue gets overidden we compare the counts from each queue
if the orignal reference count is larger than the overide queue's count then the remaining
elements from the orignal reference are appended to overide queue
*/
Class(Queue,
__INIT(u64 init_size; u64 type_size; DSN_fieldType dsn_type; void* literal;),
__FIELD(),

	interface(Formatter);
	interface(IterableList);

	errvt method(Queue,Index,,   bool write, u64 index, void* data);
	void* method(Queue,ToPointer);
	errvt method(Queue,Limit,,   u64 limit);
	void* method(Queue,FreeToPointer);
	errvt method(Queue,Reserve,, bool exact, u64 amount);
	errvt method(Queue,Enqueue,, void* item, u64 num);
	errvt method(Queue,Dequeue,, void* out, u64 num);
	u64   method(Queue,Count);
	bool  method(Queue,Check);
);



/**
@def newQueue(type, ...)
@brief allocates a static buffer object on the stack 
*/
#define newQueue(type, size) new(Queue, 		\
		size,					\
		sizeof(type),	 			\
		getDSN_Type((type){0}),			\
		NULL)
/**
@def pushQueue(type, size)
@brief allocates a static buffer object on the stack 
*/
#define pushQueue(type, size) push(Queue, 		\
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
#define l_q(first, ...) push(Queue, sizeof((typeof(first)[]){first, __VA_ARGS__})\
				 / sizeof(typeof(first)), 			 \
			   	 sizeof(typeof(first)), 			 \
				 getDSN_Type((typeof(first)){0}),		 \
				 (typeof(first)[]){first, __VA_ARGS__}		 \
			)
/**
@def L_Q(first, ...)
@brief allocates a list object literal on the heap 
@details the type is infered using typeof() from the first parameter, then the others are assumed to 
be of the same type
*/
#define L_Q(first, ...) new(Queue, sizeof((typeof(first)[]){first, __VA_ARGS__})\
				 / sizeof(typeof(first)), 			\
			   	 sizeof(typeof(first)), 			\
				 getDSN_Type((typeof(first)){0}),		\
				 (typeof(first)[]){first, __VA_ARGS__}		\
			)

#pragma once

#define __XC_DATA__ 
#include "../pkg.h"
#include "../types.h"

/**
@class List
@implements Formatter
@implements IterableList
@brief a dynamic array of any type of arbitrary size
@details This class represents a dynamic array which can be 
used wherever an unknown amount of items must be put into an array 


@subsection Constructor
@brief Initializes a Buffer object
@param init_size	the initial number of elements in the list
@param type_size	the size of a single element in the list
@param dsn_type 	the dsn_type of the element's type, this is used for DSN formating
@param literal		a pointer to an array of the same size, this data will be copied into the internal buffer


@subsection DSN
@details This is one of the basic data type in the DSN format anotated by the [...] syntax

if a list gets overidden we compare the counts from each list
if the orignal reference count is larger than the overide lists count then the remaining
elements from the orignal reference are appended to overide list
*/
Class(List,
__INIT(u64 init_size; u64 type_size; DSN_fieldType dsn_type; void* literal;),
__FIELD(),

	interface(Formatter);
	interface(IterableList);	

	errvt 		method(List,Limit,, u64 limit_size);
	errvt 		method(List,Append,, void* in, u64 len);
	errvt 		method(List,Insert,, u64 len, u64 index, void* in);
	u32 		method(List,Pop,, u32 num);
	noFail 		method(List,Flush);
	errvt 		method(List,Index,, bool write, u64 index, u64 len, void* data);
	errvt		method(List,SetFree,, u64 index);
	u64		method(List,FillSlot,, void* in);
	u64 		method(List,Size);
	void* 		method(List,GetPointer,, u64 offset);
	void* 		method(List,FreeToPointer);
	inst(List)	method(List,SubList,, u64 index, u64 len);
	errvt 		method(List,Merge,, inst(List) merged_list,u64 index);
	errvt 		method(List,Reserve,, bool exact, u64 amount);
	errvt   	method(List,Cast,, DSN_fieldType dsn_type, u64 new_type_size);
)


/**
@def newList(type, ...)
@brief allocates a static buffer object on the stack 
*/
#define newList(type, size) new(List, 			\
		size,					\
		sizeof(type),	 			\
		getDSN_Type((type){0}),			\
		NULL)
/**
@def pushList(type, size)
@brief allocates a static buffer object on the stack 
*/
#define pushList(type, size) push(List, 		\
		size,					\
		sizeof(type),	 			\
		getDSN_Type((type){0}),			\
		NULL)
/**
@def l(first, ...)
@brief allocates a list object literal on the stack 
@details the type is infered using typeof() from the first parameter, then the others are assumed to 
be of the same type
*/
#define l(first, ...) push(List, sizeof((typeof(first)[]){first, __VA_ARGS__}) 	\
				 / sizeof(typeof(first)), 			\
			   	 sizeof(typeof(first)), 			\
				 getDSN_Type((typeof(first)){0}),		\
				 (typeof(first)[]){first, __VA_ARGS__}		\
			)
/**
@def L(first, ...)
@brief allocates a list object literal on the heap 
@details the type is infered using typeof() from the first parameter, then the others are assumed to 
be of the same type
*/
#define L(first, ...) new(List, sizeof((typeof(first)[]){first, __VA_ARGS__}) 	\
				 / sizeof(typeof(first)), 			\
			   	 sizeof(typeof(first)), 			\
				 getDSN_Type((typeof(first)){0}),		\
				 (typeof(first)[]){first, __VA_ARGS__}		\
			)



#define ListCast(list, type) \
List.Cast(list, getDSN_Type((type){0}), sizeof(type))

#define ListCopy(list) List.SubList(list, 0, List.Size(list))

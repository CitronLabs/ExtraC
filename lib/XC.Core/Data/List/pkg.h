#pragma once

#define __XC_DATA__ 
#include "../pkg.h"
#include "../types.h"
#define package std

/**
@class List
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
INIT(len_t init_size; Type_t type; void* literal),
FMT(bool debug),
private(
	void* data;
	len_t items_alloced, items, limit, type_size, 
	    * free_slots_buff, free_slots_buff_alloced, free_slots_buff_size;
	std_typeData type;
	busylock lock;
)){
	errvt 		method(List, Limit, len_t limit_size);
	errvt 		method(List, Append, void* in, len_t len);
	errvt 		method(List, Insert, len_t len, u64 index, void* in);
	u32 		method(List, Pop, u32 num);
	noFail 		method(List, Flush);
	errvt 		method(List, Index, bool write, u64 index, len_t len, void* data);
	errvt		method(List, SetFree, u64 index);
	u64		method(List, FillSlot, void* in);
	len_t 		method(List, Size);
	void* 		method(List, GetPointer, u64 index);
	void* 		method(List, FreeToPointer);
	errvt 		method(List, Merge, std_List* merged_list, u64 index);
	std_List*	method(List, SubList, u64 index, len_t len);
	errvt 		method(List, Reserve, bool exact, len_t amount);
	errvt   	method(List, Cast, Type_t type);
	std_typeData	method(List, GetType);

};

/**
@def newList(type, ...)
@brief allocates a static buffer object on the stack 
*/
#define newList(_type, _size) new(std_List, _size, T(_type))
/**
@def pushList(type, size)
@brief allocates a static buffer object on the stack 
*/
#define pushList(_type, _size) push(std_List, _size, T(_type))

/**
@def l(first, ...)
@brief allocates a list object literal on the stack 
@details the type is infered using typeof() from the first parameter, then the others are assumed to 
be of the same type
*/
#define l(first, ...) 					\
push(List, 						\
	lenof((typeof(first)[]){first, __VA_ARGS__}),	\
	TFrom(asObject(first)), 			\
	(typeof(first)[]){first, __VA_ARGS__}		\
)

/**
@def L(first, ...)
@brief allocates a list object literal on the heap 
@details the type is infered using typeof() from the first parameter, then the others are assumed to 
be of the same type
*/
#define L(first, ...) 					\
new(List, 						\
	lenof((typeof(first)[]){first, __VA_ARGS__}),	\
	TFrom(asObject(first)), 			\
	(typeof(first)[]){first, __VA_ARGS__}		\
)





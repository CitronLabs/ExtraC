#pragma once
#define __XC_FEATURE__
#include "../pkg.h"
#include "../Types/pkg.h"

#define __APPEND(class)	 u64   imethodimpl(Class, _APPEND,, pntr data[])			
#define __GET(class)	 u64   imethodimpl(Class, _APPEND,, pntr data[])			
#define __SET(class)     errvt imethodimpl(Class, _SET,,    void* value)			
#define __COPY(class)    errvt imethodimpl(Class, _COPY,,   void* where)			
#define __HASH(class)    errvt imethodimpl(Class, _HASH,,   u32* result)				
#define __ITER(class)    void* imethodimpl(Class, _ITER,,   u64 index)				
#define __SCAN(class)    u64   imethodimpl(Class, _SCAN,,   FormatID* format,  str_t* in)	
#define __PRINT(class)   u64   imethodimpl(Class, _PRINT,,  FormatID* format,  strbuff_t* out)	
 

#define append(var, ...)  (var)->__ops->Append(var, (void*[]){__VA_ARGS__})
#define get(var, ...)  	  (var)->__ops->Get(var, (void*[]){__VA_ARGS__})
#define set(var, to)      (var)->__ops->Set(var, to)
#define copy(var, to)     (var)->__ops->Copy(var, to)
#define elements(var)     (var)->__ops->Size(var, true)
#define hash(var)         (var)->__ops->Hash(var)

#define hash_bytes(pntr, size) __XC_Default_Hash(pntr, size)	



/**
@def foreach(iterable, type, var)
@param iterable any object who's class has implemented the __ITER interface
@param type the type of a single member within the iterable object
@param var the name of the varible which stores the current member of the iteration
*/
#define foreach(iterableVar, _type, _var) 					\
	TypeOperators* _var##_ops = iterableVar->__ops;				\
	void*          _var##_iterable = iterableVar;				\
	_type* _var = NULL;							\
	for(size_t _var##_iterator = 0; 					\
     	    (_var = _var##_ops->						\
		Iter(_var##_iterable, _var##_iterator)); 			\
	    	_var##_iterator++)						\
	



#define HASHMAP_HASH_INIT 2166136261u
static inline u32 __XC_Default_Hash(void* data_ptr, u64 size){

	char* data = data_ptr;
	size_t nblocks = size / 8;
	u64 hash = HASHMAP_HASH_INIT;
	for (size_t i = 0; i < nblocks; ++i)
	{
		hash ^=  (u64)data[0] << 0  | (u64)data[1] << 8  |
			 (u64)data[2] << 16 | (u64)data[3] << 24 |
			 (u64)data[4] << 32 | (u64)data[5] << 40 |
			 (u64)data[6] << 48 | (u64)data[7] << 56;
		hash *= 0xbf58476d1ce4e5b9;
		data += 8;
	}

	u64 last = size & 0xff;
	switch (size % 8)
	{
	case 7:
		last |= (u64)data[6] << 56; 
	case 6:
		last |= (u64)data[5] << 48; 
	case 5:
		last |= (u64)data[4] << 40; 
	case 4:
		last |= (u64)data[3] << 32; 
	case 3:
		last |= (u64)data[2] << 24; 
	case 2:
		last |= (u64)data[1] << 16; 
	case 1:
		last |= (u64)data[0] << 8;
		hash ^= last;
		hash *= 0xd6e8feb86659fd93;
	}

return (u32)(hash ^ hash >> 32);
}

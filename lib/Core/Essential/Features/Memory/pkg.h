#pragma once
#define __XC_FEATURE__
#include "../pkg.h"

#include "../Type/pkg.h"

#define package std

#include "utils.h"

Class(Memory,
INIT(len_t size), 
FMT(), 
      pntr pointer;
      len_t size;
){
	bool  fn(compare)(void* a, void* b, len_t size);
	void* fn(setTo)(void* dest, int val, len_t size);
	void* fn(copyTo)(void* dest, void* from, len_t size);
	void* fn(alloc)(len_t size);
	void* fn(grow)(void* data, len_t size);
	void  fn(dealloc)(void* data);

};

#include "./config.c"

#define alloca(size) 			__MEMORY_STACK_ALLOC(size)
#define malloc(size) 			std.Memory.alloc(size)
#define realloc(pntr, size) 		std.Memory.grow(pntr, size)
#define free(pntr) 			std.Memory.dealloc(pntr)
#define memcpy(dest, from,  size) 	std.Memory.copyTo(dest, from, size)
#define memset(dest, value, size) 	std.Memory.setTo(dest, value, size)
#define memcmp(a, b,  size) 		std.Memory.compare(a, b, size)
#define calloc(num, size) 		memset(malloc(num * size), 0, num * size)

#undef package

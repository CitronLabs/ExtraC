#pragma once
#define __XC_FEATURE__
#include "../pkg.h"

#include "../Classes/pkg.h"
#include "../Type/pkg.h"

#define package std

Class(Memory,
INIT(len_t size), 
FMT(), 
      pntr pointer;
      len_t size;
){
	bool  fn(compare)(void* a, void* b, len_t size);
	void* fn(setTo)(void* dest, int val, len_t size);
	void* fn(copyTo)(void* dest, void* from, len_t size);
	void* fn(heapAlloc)(len_t size);
	void* fn(stackAlloc)(len_t size);
	void* fn(grow)(void* data, len_t size);
	void  fn(heapDealloc)(void* data);

};

#define malloc(size) 			std.Memory.heapAlloc(size)
#define alloca(size) 			std.Memory.stackAlloc(size)
#define realloc(pntr, size) 		std.Memory.grow(pntr, size)
#define free(pntr) 			std.Memory.heapDealloc(pntr)
#define memcpy(dest, from,  size) 	std.Memory.copyTo(dest, from, size)
#define memset(dest, value, size) 	std.Memory.setTo(dest, value, size)
#define memcmp(a, b,  size) 		std.Memory.compare(a, b, size)

#undef package

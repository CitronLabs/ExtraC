#pragma once
#define __XC_FEATURE__
#include "../pkg.h"

#include "../Type/pkg.h"

#define package std

#include "utils.h"

Interface(Allocator,
	void* imethod(Alloc, u64 size);
      	errvt imethod(Free, void* instance);
      	void* imethod(Resize, void* instance, u64 size);
      	errvt imethod(setMax, u64 size);
      	bool  imethod(isStatic);
      	u64   imethod(getBytesAlloced);
);

Class(Memory,
INIT(len_t size), 
FMT(), 
      pntr pointer;
      len_t size;
){
	interface(std_Allocator) Allocator;

	bool  fn(compare)(void* a, void* b, len_t size);
	void* fn(setTo)(void* dest, int val, len_t size);
	void* fn(copyTo)(void* dest, void* from, len_t size);

	std_Memory* fn(getHeap)();
};

#include "./config.c"

#define new_use(name, allocator, ...) \
	create(name, (allocator)->Alloc(sizeof(name))__VA_OPT__(, __VA_ARGS__))

#define new_use_with(name, constructor, allocator, ...) \
	create_with(name, constructor, (allocator)->Alloc(sizeof(name))__VA_OPT__(, __VA_ARGS__))

#define del_use(allocator, ...) \
	std.Type.alloc.destructWith((allocator)->Free, (pntr[]){__VA_ARGS__}, sizeof((pntr[]){__VA_ARGS__}) / sizeof(pntr))


#define alloca(size) 			__MEMORY_STACK_ALLOC(size)
#define malloc(size) 			std.Memory.Allocator.Alloc (std.Memory.getHeap(), size)
#define realloc(pntr, size) 		std.Memory.Allocator.Resize(std.Memory.getHeap(), pntr, size)
#define free(pntr) 			std.Memory.Allocator.Free  (std.Memory.getHeap(), pntr)
#define memcpy(dest, from,  size) 	std.Memory.copyTo(dest, from, size)
#define memset(dest, value, size) 	std.Memory.setTo(dest, value, size)
#define memcmp(a, b,  size) 		std.Memory.compare(a, b, size)
#define calloc(num, size) 		memset(malloc(num * size), 0, num * size)

#undef package

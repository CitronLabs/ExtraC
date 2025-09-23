#pragma once

#define __XC_ALLOC__
#include "../pkg.h"
#include "../types.h"

/**
@class Allocator
@brief A simple interface over core allocator functions
*/
Interface(Allocator,
	void* imethod(New,,    u64   size,     void* ex_args);
      	errvt imethod(Delete,, void* instance, void* ex_args);
      	void* imethod(Resize,, void* instance, u64 size, void* ex_args);
      	errvt imethod(setMax,, u64   size);
      	bool  imethod(isStatic);
      	u64   imethod(getBytesAlloced);
);

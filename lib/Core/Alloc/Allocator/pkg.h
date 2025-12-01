#pragma once

#define __XC_ALLOC__
#include "../pkg.h"
#include "../types.h"

#define package std
/**
@class Allocator
@brief A simple interface over core allocator functions
*/
Interface(Allocator,
	void* imethod(New, u64 size);
      	errvt imethod(Delete, void* instance);
      	void* imethod(Resize, void* instance, u64 size);
      	errvt imethod(setMax, u64 size);
      	bool  imethod(isStatic);
      	u64   imethod(getBytesAlloced);
);

#undef package

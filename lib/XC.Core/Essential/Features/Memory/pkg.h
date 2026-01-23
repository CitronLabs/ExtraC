#pragma once
#define __XC_FEATURE__
#include "../pkg.h"

#include "../Type/pkg.h"
#include "../XCTypes/pkg.h"
#include "../Error/pkg.h"

#ifndef __XC_MEMORY__
#include "types.h"
#include "utils.h"

#define package std

Interface(Allocator,
	void* imethod(Alloc, u64 size, std_ErrorPosition errorPos);
      	errvt imethod(Free, void* instance, std_ErrorPosition errorPos);
      	void* imethod(Resize, void* instance, u64 size, std_ErrorPosition errorPos);
      	errvt imethod(setMax, u64 size);
      	bool  imethod(isStatic);
      	u64   imethod(getBytesAlloced);
);

Class(Memory,
INIT(len_t size), 
FMT(), 
      	pntr  pointer;
      	len_t pages;
private(bool managed;)
){
     	struct {
	    values(Optimize, uword,
    		SPEED,      /* Minimize allocation time */
    		SIZE,      /* Minimize fragmentation */
    		SECURITY    /* Maximum security features */
	    )

	    const pkg(Memory_Allocator_Settings) defaultSettings;

	    pkg(Memory_Allocator_Settings) fn(optimizeSettings)(uword flag);
	    errvt method(Memory, setup, pkg(Memory_Allocator_Settings) settings);

	    interface(std_Allocator) Interface;
	    struct {
		errvt 
		method(Memory, settings, pkg(Memory_Allocator_Settings) settings),
		method(Memory, optimization, uword flag),
		method(Memory, setMinSplitThreshold, len_t value),
		method(Memory, setLargeMmapThreshold, len_t value),
		method(Memory, setVerboseErrors, bool value),
		method(Memory, enableZeroOnFree, bool enable),
		method(Memory, enablePoisonOnFree, bool enable),
		method(Memory, enableCanaries, bool enable),
		method(Memory, enableValidation, bool enable),
		method(Memory, enableQuarantine, bool enable, len_t size),
		method(Memory, enableDeferredCoalescing, bool enable),
		method(Memory, setMaxHeapSize, len_t max);
	    } Edit;
     	} Allocator;

	bool  fn(compare)(void* a, void* b, len_t size);
	void* fn(setTo)(void* dest, int val, len_t size);
	void* fn(copyTo)(void* dest, void* from, len_t size);

	std_Memory* fn(getHeap)();
};


#undef package
#endif

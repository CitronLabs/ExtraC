#pragma once
#include "../../config.c"


#define __MEMORY_STACK_ALLOC(len) __builtin_alloca(len)
#define __INIT_HEAP_NUM_PAGES	  4
#define __INIT_HEAP_LOCATION	  nil

#define MEM_ALLOC_GROWTH_FACTOR(current_size) (current_size + (current_size / 2))
#define __INCLUDE_ALLOCATOR true


#define __DEFAULT_ALLOC_SETTINGS			\
    .binCount 	 		= 32,			\
    .alignment 	 		= 16,			\
    .ensureContiguous  		= false,		\
    .allowRuntimeTuning 	= true,			\
    .enableTelemetry 		= true,			\
    .maxSize 	 		= ALLOC_SIZE_UNLIMITED,	\
    .verboseErrors 	 	= true,			\
    .minSplitThreshold 		= 48,			\
    .zeroOnFree 		= false,		\
    .poisonOnFree 		= false,		\
    .useCanaries 		= true,			\
    .validateOnEntry 		= false,		\
    .useRandomCanaries 		= false,		\
    .useQuarantine 		= false,		\
    .quarantineSize 		= 0,			\
    .useDeferredCoalescing 	= false,		\
    .enableThreadCache 		= false,		\
    .largeMmapThreshold 	= 192 * 1024,		\
    .trackCallSites 		= true

#pragma once
#define __XC_FEATURE__
#include "../pkg.h"

#include "../Type/pkg.h"
#include "../Error/pkg.h"

#define package std

#include "utils.h"

Interface(Allocator,
	void* imethod(Alloc, u64 size, std_ErrorPosition errorPos);
      	errvt imethod(Free, void* instance, std_ErrorPosition errorPos);
      	void* imethod(Resize, void* instance, u64 size, std_ErrorPosition errorPos);
      	errvt imethod(setMax, u64 size);
      	bool  imethod(isStatic);
      	u64   imethod(getBytesAlloced);
);

type(Memory_AllocSettings,
    u32 bin_count;
    len_t min_split_threshold;
    len_t alignment;
    len_t maxMetadataPages;
    bool  ensureContiguous;
    bool  allowRuntimeTuning;
    bool  enableTelemetry;
    len_t maxHeapSize;
    
    /* Security & Debugging Features */
    bool  zeroOnFree;
    bool  poisonOnFree;
    bool  useCanaries;
    bool  validateOnEntry;
    bool  useRandomCanaries;     /* Random canary per allocation */
    bool  useQuarantine;         /* Delay reuse of freed blocks */
    len_t quarantineSize;       /* Max quarantine bytes */
    
    /* Performance Features */
    bool  useDeferredCoalescing; /* Delay coalescing for performance */
    bool  enableThreadCache;     /* Thread-local caching (future) */
    len_t largeMmapThreshold;   /* Direct mmap for large allocations */
    
    /* Diagnostic Features */
    bool  trackCallSites;        /* Track file/line of allocations */
    bool  verboseErrors;         /* Detailed error messages */


)

Class(Memory,
INIT(len_t size), 
FMT(), 
      pntr pointer;
      len_t size;
){
     	struct {
     	    values(Strategy, uword,
    		SEGREGATED_FIT,  /* Fast O(1) lookup */
    		BEST_FIT,        /* Space-efficient, uses RB-tree */
    		FIRST_FIT        /* Fast, less fragmentation than segregated */
	    )
	    values(Optimize, uword,
    		DEFAULT,    /* Default settings */
    		SPEED,      /* Minimize allocation time */
    		SPACE,      /* Minimize fragmentation */
    		BALANCED,   /* Balance speed and space */
    		SECURITY    /* Maximum security features */
	    )
	    pkg(Memory_AllocSettings) fn(optimizeSettings)(uword flag);
	    errvt fn(setup)(pkg(Memory)* mem, pkg(Memory_AllocSettings) settings);
	    void* fn(allocAligned)(len_t size, len_t alignment);

	    interface(std_Allocator) Interface;

     	} Allocator;

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


#define alloca(size)  __MEMORY_STACK_ALLOC(size)
#define malloc(size) std.Memory.Allocator.Interface.Alloc (std.Memory.getHeap(), size, getErrorPos())
#define realloc(pntr, size) std.Memory.Allocator.Interface.Resize(std.Memory.getHeap(), pntr, size, getErrorPos())
#define free(pntr) std.Memory.Allocator.Interface.Free  (std.Memory.getHeap(), pntr, getErrorPos())
#define memcpy(dest, from,  size) std.Memory.copyTo(dest, from, size)
#define memset(dest, value, size) std.Memory.setTo(dest, value, size)
#define memcmp(a, b,  size) std.Memory.compare(a, b, size)
#define calloc(num, size) memset(malloc(num * size), 0, num * size)

#undef package

#pragma once
#define __XC_MEMORY__
#include "pkg.h"

#define package std_Memory

typefrom(uword, Allocator_OptimizeMode)

type(Allocator_Settings,
    pkg(Allocator_OptimizeMode) mode;
    u32 binCount;                   /* Number of segregated free list bins */
    len_t minSplitThreshold;        /* Minimum size for splitting blocks */
    len_t alignment;                /* Memory alignment (must be power of 2) */
    bool ensureContiguous;          /* Keep heap contiguous (requires relocation) */
    bool allowRuntimeTuning;        /* Allow settings changes after init */
    bool enableTelemetry;           /* Track detailed statistics */
    len_t maxSize;                  /* Maximum size (0 = unlimited) */
    
    /* Security & Debugging Features */
    bool zeroOnFree;                 /* Clear memory on free (security) */
    bool poisonOnFree;               /* Fill freed memory with 0xDE (debugging) */
    bool useCanaries;                /* Guard bands around allocations */
    bool validateOnEntry;            /* Validate heap integrity on every call */
    bool useRandomCanaries;          /* Use random canary values per allocation */
    bool useQuarantine;              /* Delay reuse of freed blocks */
    len_t quarantineSize;            /* Maximum quarantine capacity in bytes */
    
    /* Performance Features */
    bool useDeferredCoalescing;      /* Batch coalescing operations */
    bool enableThreadCache;          /* Thread-local caching */
    len_t largeMmapThreshold;        /* Direct mmap for allocations >= this size */
    
    /* Diagnostic Features */
    bool trackCallSites;             /* Record file/line for each allocation */
    bool verboseErrors;              /* Detailed error messages */
)

type(Allocator_Telemetry,
    /* Memory Usage */
    len_t totalAllocated;            /* Total bytes allocated (user-requested) */
    len_t totalMetadataBytes;        /* Bytes used for metadata */
    len_t totalOverhead;             /* Internal overhead (headers, canaries, etc.) */
    len_t metadataPageCount;         /* Number of pages used */
    
    /* Operation Counts */
    len_t allocationCount;           /* Total allocations */
    len_t freeCount;                 /* Total frees */
    len_t reallocCount;              /* Total reallocs */
    len_t activeAllocations;         /* Current active allocations */
    len_t peakActiveAllocations;     /* Maximum concurrent allocations */
    
    /* Performance Metrics */
    len_t peakUsage;                 /* Peak memory usage in bytes */
    len_t fragmentationBytes;        /* Estimated fragmentation */
    len_t largeAllocations;          /* Count of direct mmap allocations */
    len_t coalescingEvents;          /* Number of block merges */
    len_t splittingEvents;           /* Number of block splits */
    len_t growthEvents;              /* Number of expansions */
    len_t deferredCoalescingRuns;    /* Deferred coalescing batch operations */
    
    /* Security & Debugging */
    len_t corruptionEvents;          /* Detected corruptions */
    len_t validationFailures;        /* Validation check failures */
    len_t quarantinedBlocks;         /* Blocks currently in quarantine */
    len_t quarantinedBytes;          /* Bytes currently quarantined */
    len_t settingsChangeCount;       /* Runtime settings modifications */
)

#undef package

/* allocator.h - Production Memory Allocator Header */
#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/* === TYPE DEFINITIONS === */

typedef uint64_t len_t;

/* Error Codes */
typedef enum {
    ERR_NONE = 0,
    ERR_OUT_OF_MEMORY,
    ERR_METADATA_CAP_EXCEEDED,
    ERR_HEAP_LIMIT_REACHED,
    ERR_INVALID_POINTER,
    ERR_DOUBLE_FREE,
    ERR_CORRUPTION_DETECTED,
    ERR_ALIGNMENT_FAILURE,
    ERR_RELOCATION_FAILED,
    ERR_INVALID_SIZE,
    ERR_SIZE_OVERFLOW,
    ERR_UNALIGNED_BASE,
    ERR_INVALID_SETTINGS,
    ERR_RUNTIME_TUNING_DISABLED,
    ERR_INCOMPATIBLE_SETTINGS,
    ERR_LARGE_ALLOC_FAILED,
} AllocError;

/* Allocation Strategies */
typedef enum {
    STRATEGY_SEGREGATED_FIT = 0,  /* Fast O(1) bin lookup */
    STRATEGY_BEST_FIT = 1,        /* Space-efficient, searches for best fit */
    STRATEGY_FIRST_FIT = 2        /* Fast, returns first suitable block */
} AllocStrategy;

/* Optimization Modes */
typedef enum {
    OPTIMIZE_SPEED = 0,      /* Minimize allocation time */
    OPTIMIZE_SPACE = 1,      /* Minimize fragmentation and memory waste */
    OPTIMIZE_BALANCED = 2,   /* Balance between speed and space */
    OPTIMIZE_SECURITY = 3    /* Maximum security features enabled */
} OptimizationMode;

/* Allocator Settings */
typedef struct {
    /* Core Configuration */
    AllocStrategy strategy;
    OptimizationMode mode;
    uint32_t bin_count;              /* Number of segregated free list bins */
    size_t min_split_threshold;      /* Minimum size for splitting blocks */
    size_t alignment;                /* Memory alignment (must be power of 2) */
    len_t maxMetadataPages;          /* Max pages for metadata */
    bool ensureContiguous;           /* Keep heap contiguous (requires relocation) */
    bool allowRuntimeTuning;         /* Allow settings changes after init */
    bool enableTelemetry;            /* Track detailed statistics */
    len_t maxHeapSize;               /* Maximum heap size (0 = unlimited) */
    
    /* Security & Debugging Features */
    bool zeroOnFree;                 /* Clear memory on free (security) */
    bool poisonOnFree;               /* Fill freed memory with 0xDE (debugging) */
    bool useCanaries;                /* Guard bands around allocations */
    bool validateOnEntry;            /* Validate heap integrity on every call */
    bool useRandomCanaries;          /* Use random canary values per allocation */
    bool useQuarantine;              /* Delay reuse of freed blocks */
    size_t quarantineSize;           /* Maximum quarantine capacity in bytes */
    
    /* Performance Features */
    bool useDeferredCoalescing;      /* Batch coalescing operations */
    bool enableThreadCache;          /* Thread-local caching (future) */
    size_t largeMmapThreshold;       /* Direct mmap for allocations >= this size */
    
    /* Diagnostic Features */
    bool trackCallSites;             /* Record file/line for each allocation */
    bool verboseErrors;              /* Detailed error messages */
    void (*errorCallback)(AllocError err, const char* msg, void* userdata);
    void* callbackUserdata;          /* User data passed to error callback */
} MemAllocSettings;

/* Telemetry Statistics */
typedef struct {
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
    len_t heapGrowthEvents;          /* Number of heap expansions */
    len_t deferredCoalescingRuns;    /* Deferred coalescing batch operations */
    
    /* Security & Debugging */
    len_t corruptionEvents;          /* Detected corruptions */
    len_t validationFailures;        /* Validation check failures */
    len_t quarantinedBlocks;         /* Blocks currently in quarantine */
    len_t quarantinedBytes;          /* Bytes currently quarantined */
    len_t settingsChangeCount;       /* Runtime settings modifications */
    
    /* Error Information */
    AllocError lastError;            /* Last error code */
    char lastErrorMsg[256];          /* Last error message */
} AllocatorTelemetry;

/* Internal Block Header */
typedef struct BlockHeader {
    uint32_t magic;                  /* Magic number for validation */
    uint32_t canary_top;             /* Top canary for overflow detection */
    len_t size;                      /* Total block size including metadata */
    len_t requested_size;            /* Original user-requested size */
    int is_free;                     /* 1 if free, 0 if allocated */
    intptr_t rel_next_phys;          /* Relative pointer to next physical block */
    intptr_t rel_prev_phys;          /* Relative pointer to previous physical block */
    
    /* Debug Information (only if trackCallSites enabled) */
    const char* alloc_file;          /* Source file of allocation */
    int alloc_line;                  /* Source line of allocation */
    uint64_t alloc_timestamp;        /* Allocation sequence number */
} BlockHeader;

/* Free Block (extends BlockHeader) */
typedef struct FreeBlock {
    BlockHeader header;
    intptr_t rel_next_free;          /* Relative pointer to next free block in bin */
    intptr_t rel_prev_free;          /* Relative pointer to previous free block in bin */
} FreeBlock;

/* Large Allocation Tracking */
typedef struct LargeAlloc {
    void* ptr;                       /* Pointer to allocated memory */
    size_t size;                     /* Total size allocated */
    size_t requested_size;           /* User-requested size */
    const char* alloc_file;          /* Source file */
    int alloc_line;                  /* Source line */
    uint64_t alloc_timestamp;        /* Allocation timestamp */
    struct LargeAlloc* next;         /* Next large allocation */
} LargeAlloc;

/* Quarantine Node */
typedef struct QuarantineNode {
    void* block_ptr;                 /* Pointer to BlockHeader */
    size_t size;                     /* Block size */
    uint64_t timestamp;              /* When quarantined */
    struct QuarantineNode* next;     /* Next in queue */
} QuarantineNode;

/* Quarantine Structure */
typedef struct Quarantine {
    QuarantineNode* head;            /* First node */
    QuarantineNode* tail;            /* Last node */
    size_t currentBytes;             /* Current quarantined bytes */
    size_t maxBytes;                 /* Maximum quarantine size */
    size_t nodeCount;                /* Number of quarantined blocks */
} Quarantine;

/* Deferred Coalescing List */
typedef struct DeferredBlock {
    BlockHeader* block;              /* Block to coalesce */
    struct DeferredBlock* next;      /* Next deferred block */
} DeferredBlock;

/* Main Allocator Context */
typedef struct AllocatorContext {
    void* base_addr;                 /* Base address of heap */
    len_t total_size;                /* Total heap size */
    bool managed;                    /* True if fixed-size, false if growable */
    
    MemAllocSettings settings;       /* Current settings */
    AllocatorTelemetry telemetry;    /* Statistics */
    
    uint64_t alloc_counter;          /* Monotonic allocation counter */
    uint32_t canary_seed;            /* Seed for random canaries */
    
    Quarantine quarantine;           /* Quarantine structure */
    LargeAlloc* large_allocs;        /* List of large allocations */
    DeferredBlock* deferred_list;    /* Deferred coalescing list */
    size_t deferred_count;           /* Count of deferred blocks */
    size_t deferred_threshold;       /* Threshold to trigger coalescing */
    
    intptr_t bin_offsets[];          /* Flexible array of free list bins */
} AllocatorContext;

/* === CONSTANTS === */

#define CANARY_VALUE_BASE    0xABADCAFE
#define POISON_VALUE         0xDEADBEEF
#define FREE_POISON          0xFEEEFEEE
#define MAGIC_ALLOCATED      0xA110CA7E
#define MAGIC_FREE           0xFEEEFEEE
#define MAGIC_LARGE          0x1A6EA110

/* === MACROS === */

#define PTR_TO_REL(base, ptr) \
    ((ptr) ? (intptr_t)((uint8_t*)(ptr) - (uint8_t*)(base)) : -1)

#define REL_TO_PTR(base, rel) \
    ((rel) != -1 ? (void*)((uint8_t*)(base) + (rel)) : NULL)

#define ALIGN_UP(val, align) \
    (((val) + ((align) - 1)) & ~((align) - 1))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define IS_POWER_OF_TWO(x) \
    (((x) != 0) && (((x) & ((x) - 1)) == 0))

#define MIN_BLOCK_SIZE (sizeof(FreeBlock) + sizeof(uint32_t))

/* Convenience macros for allocation with automatic file/line tracking */
#define MALLOC(ctx, size) \
    std_malloc_debug(&(ctx), (size), __FILE__, __LINE__)

#define FREE(ctx, ptr) \
    std_free((ctx), (ptr))

#define CALLOC(ctx, nmemb, size) \
    std_calloc(&(ctx), (nmemb), (size))

#define REALLOC(ctx, ptr, size) \
    std_realloc(&(ctx), (ptr), (size))

#define ALIGNED_ALLOC(ctx, alignment, size) \
    std_aligned_alloc(&(ctx), (alignment), (size))

/* === PUBLIC API === */

/* Initialization and Destruction */
AllocatorContext* createAllocator(OptimizationMode mode, size_t initial_size);
AllocatorContext* initAllocator(void* base, size_t size, MemAllocSettings settings);
void destroyAllocator(AllocatorContext* ctx);

/* Memory Allocation Functions */
void* std_malloc(AllocatorContext** ctx, size_t size);
void* std_malloc_debug(AllocatorContext** ctx, size_t size, const char* file, int line);
void* std_calloc(AllocatorContext** ctx, size_t nmemb, size_t size);
void* std_realloc(AllocatorContext** ctx, void* ptr, size_t size);
void* std_aligned_alloc(AllocatorContext** ctx, size_t alignment, size_t size);
void std_free(AllocatorContext* ctx, void* ptr);
void std_free_diag(AllocatorContext* ctx, void* ptr);

/* Runtime Settings Modification */
AllocError applyRuntimeSettings(AllocatorContext* ctx, MemAllocSettings* new_settings);
AllocError switchOptimizationMode(AllocatorContext* ctx, OptimizationMode new_mode);

/* Individual Setting Modifiers */
AllocError setStrategy(AllocatorContext* ctx, AllocStrategy strategy);
AllocError setMinSplitThreshold(AllocatorContext* ctx, size_t threshold);
AllocError setLargeMmapThreshold(AllocatorContext* ctx, size_t threshold);
AllocError setMaxHeapSize(AllocatorContext* ctx, len_t max_size);
AllocError setVerboseErrors(AllocatorContext* ctx, bool enable);

AllocError enableZeroOnFree(AllocatorContext* ctx, bool enable);
AllocError enablePoisonOnFree(AllocatorContext* ctx, bool enable);
AllocError enableCanaries(AllocatorContext* ctx, bool enable);
AllocError enableValidation(AllocatorContext* ctx, bool enable);
AllocError enableQuarantine(AllocatorContext* ctx, bool enable, size_t size);
AllocError enableDeferredCoalescing(AllocatorContext* ctx, bool enable);

AllocError setErrorCallback(AllocatorContext* ctx,
                            void (*callback)(AllocError, const char*, void*),
                            void* userdata);

/* Query Functions */
MemAllocSettings getCurrentSettings(AllocatorContext* ctx);
const char* getAllocatorError(AllocatorContext* ctx);
AllocError getLastErrorCode(AllocatorContext* ctx);
void clearError(AllocatorContext* ctx);
bool canModifySetting(AllocatorContext* ctx, const char* setting_name);

/* Diagnostic Functions */
void dump_heap_stats(AllocatorContext* ctx, FILE* out);
void dump_allocations(AllocatorContext* ctx, FILE* out);
void printDetailedStats(AllocatorContext* ctx, FILE* out);

/* System Interface (must be provided by user) */
extern void* allocPages(size_t size);
extern void freePages(void* ptr, size_t size);
extern size_t getPageSize(void);

#endif /* ALLOCATOR_H */


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

/* --- System Interface Requirements --- */
extern void* allocPages(size_t size);
extern void  freePages(void* ptr, size_t size);
extern size_t getPageSize(void);

typedef uint64_t len_t;

/* --- Enhanced Error Codes with Detailed Messages --- */

typedef struct {
    AllocStrategy strategy;
    uint32_t bin_count;
    size_t min_split_threshold;
    size_t alignment;
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
    size_t quarantineSize;       /* Max quarantine bytes */
    
    /* Performance Features */
    bool  useDeferredCoalescing; /* Delay coalescing for performance */
    bool  enableThreadCache;     /* Thread-local caching (future) */
    size_t largeMmapThreshold;   /* Direct mmap for large allocations */
    
    /* Diagnostic Features */
    bool  trackCallSites;        /* Track file/line of allocations */
    bool  verboseErrors;         /* Detailed error messages */
} MemAllocSettings;

typedef struct {
    len_t totalAllocated;
    len_t totalMetadataBytes;
    len_t totalOverhead;
    len_t metadataPageCount;
    len_t allocationCount;
    len_t freeCount;
    len_t reallocCount;
    len_t activeAllocations;
    len_t peakUsage;
    len_t peakActiveAllocations;
    len_t corruptionEvents;
    len_t fragmentationBytes;
    len_t largeAllocations;      /* Direct mmap allocations */
    len_t coalescingEvents;
    len_t splittingEvents;
    len_t heapGrowthEvents;
    len_t quarantinedBlocks;
    len_t quarantinedBytes;
    AllocError lastError;
    char lastErrorMsg[256];
} AllocatorTelemetry;

/* --- Internal Structures --- */

#define CANARY_VALUE_BASE 0xABADCAFE
#define POISON_VALUE 0xDEADBEEF
#define FREE_POISON 0xFEEEFEEE
#define MAGIC_ALLOCATED 0xA110CA7E
#define MAGIC_FREE 0xFEEEFEEE

/* Compile-time assertion for power of 2 */
#define IS_POWER_OF_TWO(x) (((x) != 0) && (((x) & ((x) - 1)) == 0))

typedef struct BlockHeader {
    uint32_t magic;               /* MAGIC_ALLOCATED or MAGIC_FREE */
    uint32_t canary_top;          /* Security: detect header smash */
    len_t size;                   /* Total block size including header/canary */
    len_t requested_size;         /* Original user request (for diagnostics) */
    int is_free;
    intptr_t rel_next_phys;       /* Relative to this header */
    intptr_t rel_prev_phys;       /* Relative to this header */
    
    /* Debug info (only if trackCallSites enabled) */
    const char* alloc_file;
    int alloc_line;
    uint64_t alloc_timestamp;     /* Allocation sequence number */
} BlockHeader;

typedef struct FreeBlock {
    BlockHeader header;
    intptr_t rel_next_free;       /* Relative to AllocatorContext base */
    intptr_t rel_prev_free;       /* Relative to AllocatorContext base */
} FreeBlock;

/* Quarantine for use-after-free protection */
typedef struct QuarantineNode {
    void* ptr;
    size_t size;
    struct QuarantineNode* next;
} QuarantineNode;

typedef struct Quarantine {
    QuarantineNode* head;
    size_t currentBytes;
    size_t maxBytes;
} Quarantine;

/* Red-Black tree node for best-fit allocation */
typedef enum { RB_RED, RB_BLACK } RBColor;

typedef struct RBTreeNode {
    BlockHeader* block;
    RBColor color;
    intptr_t rel_left;
    intptr_t rel_right;
    intptr_t rel_parent;
} RBTreeNode;

typedef struct AllocatorContext {
    void* base_addr;
    len_t total_size;
    bool  managed;
    MemAllocSettings settings;
    AllocatorTelemetry telemetry;
    uint64_t alloc_counter;       /* Monotonic allocation counter */
    uint32_t canary_seed;         /* For random canaries */
    Quarantine quarantine;
    intptr_t rb_tree_root;        /* For best-fit strategy */
    intptr_t deferred_coalesce_list; /* Blocks pending coalescing */
    intptr_t bin_offsets[];       /* Flexible array of bin heads */
} AllocatorContext;

/* --- Macros --- */
#define PTR_TO_REL(base, ptr) ((ptr) ? (intptr_t)((uint8_t*)(ptr) - (uint8_t*)(base)) : -1)
#define REL_TO_PTR(base, rel) ((rel) != -1 ? (void*)((uint8_t*)(base) + (rel)) : NULL)
#define ALIGN_UP(val, align)  (((val) + ((align) - 1)) & ~((align) - 1))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Minimum block size must fit FreeBlock */
#define MIN_BLOCK_SIZE (sizeof(FreeBlock) + sizeof(uint32_t))

/* --- Error Reporting --- */

static const char* error_messages[] = {
    [ERR_NONE] = "No error",
    [ERR_OUT_OF_MEMORY] = "Out of memory: system cannot allocate more pages",
    [ERR_METADATA_CAP_EXCEEDED] = "Metadata capacity exceeded: hit maxMetadataPages limit",
    [ERR_HEAP_LIMIT_REACHED] = "Heap limit reached: allocation would exceed maxHeapSize",
    [ERR_INVALID_POINTER] = "Invalid pointer: pointer not from this allocator or corrupted",
    [ERR_DOUBLE_FREE] = "Double free detected: attempting to free already-freed memory",
    [ERR_CORRUPTION_DETECTED] = "Memory corruption detected: canary value mismatch or invalid magic",
    [ERR_ALIGNMENT_FAILURE] = "Alignment failure: requested alignment not power of 2 or too large",
    [ERR_RELOCATION_FAILED] = "Heap relocation failed: cannot ensure contiguous memory",
    [ERR_INVALID_SIZE] = "Invalid size: allocation size is 0 or negative",
    [ERR_SIZE_OVERFLOW] = "Size overflow: requested size + overhead exceeds maximum",
    [ERR_UNALIGNED_BASE] = "Unaligned base: heap base address not properly aligned",
    [ERR_INVALID_SETTINGS] = "Invalid settings: configuration parameters are inconsistent",
    [ERR_THREAD_SAFETY_VIOLATION] = "Thread safety violation: concurrent access without locking",
    [ERR_REALLOC_FAILED] = "Realloc failed: cannot resize allocation",
    [ERR_QUARANTINE_FULL] = "Quarantine full: cannot quarantine more blocks"
};

static void set_error(AllocatorContext* ctx, AllocError err, const char* fmt, ...) {
    ctx->telemetry.lastError = err;
    
    if (err == ERR_CORRUPTION_DETECTED) {
        ctx->telemetry.corruptionEvents++;
    }
    
    if (ctx->settings.verboseErrors && fmt) {
        va_list args;
        va_start(args, fmt);
        vsnprintf(ctx->telemetry.lastErrorMsg, sizeof(ctx->telemetry.lastErrorMsg), fmt, args);
        va_end(args);
    } else {
        snprintf(ctx->telemetry.lastErrorMsg, sizeof(ctx->telemetry.lastErrorMsg), 
                 "%s", error_messages[err]);
    }
    
    if (ctx->settings.errorCallback) {
        ctx->settings.errorCallback(err, ctx->telemetry.lastErrorMsg, ctx);
    }
}

/* Simple pseudo-random for canaries */
static uint32_t xorshift32(uint32_t* state) {
    uint32_t x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

static uint32_t get_canary(AllocatorContext* ctx) {
    if (ctx->settings.useRandomCanaries) {
        return xorshift32(&ctx->canary_seed);
    }
    return CANARY_VALUE_BASE;
}

/* --- Block Validation --- */

static bool validate_block_internal(AllocatorContext* ctx, BlockHeader* h, bool check_canary) {
    /* Check magic number */
    if (h->magic != MAGIC_ALLOCATED && h->magic != MAGIC_FREE) {
        set_error(ctx, ERR_CORRUPTION_DETECTED, 
                  "Invalid magic number at %p: expected 0x%X or 0x%X, got 0x%X",
                  (void*)h, MAGIC_ALLOCATED, MAGIC_FREE, h->magic);
        return false;
    }
    
    /* Check size sanity */
    if (h->size < sizeof(BlockHeader) || h->size > ctx->total_size) {
        set_error(ctx, ERR_CORRUPTION_DETECTED,
                  "Invalid block size at %p: %llu bytes (total heap: %llu)",
                  (void*)h, (unsigned long long)h->size, (unsigned long long)ctx->total_size);
        return false;
    }
    
    if (check_canary && ctx->settings.useCanaries) {
        /* Check top canary */
        if (h->canary_top != CANARY_VALUE_BASE && !ctx->settings.useRandomCanaries) {
            set_error(ctx, ERR_CORRUPTION_DETECTED,
                      "Top canary corrupted at %p: expected 0x%X, got 0x%X",
                      (void*)h, CANARY_VALUE_BASE, h->canary_top);
            return false;
        }
        
        /* Check bottom canary for allocated blocks */
        if (!h->is_free) {
            uint32_t* canary_bot = (uint32_t*)((uint8_t*)h + h->size - sizeof(uint32_t));
            if (*canary_bot != h->canary_top) {
                set_error(ctx, ERR_CORRUPTION_DETECTED,
                          "Bottom canary corrupted at %p: expected 0x%X, got 0x%X (possible buffer overflow)",
                          (void*)h, h->canary_top, *canary_bot);
                return false;
            }
        }
    }
    
    return true;
}

static bool validate_block(AllocatorContext* ctx, BlockHeader* h) {
    return validate_block_internal(ctx, h, true);
}

/* --- Bin Management (Segregated Fit) --- */

static int get_bin_index(AllocatorContext* ctx, size_t size) {
    if (size <= ctx->settings.alignment) return 0;
    
    /* Use log2 to determine bin */
    int bit = 0;
    size_t temp = size - 1;
    while (temp > 0) {
        bit++;
        temp >>= 1;
    }
    
    /* Adjust for minimum bin size */
    int index = bit - 4; /* Bins: 16, 32, 64, 128, 256, ... */
    if (index < 0) index = 0;
    if (index >= (int)ctx->settings.bin_count) index = ctx->settings.bin_count - 1;
    
    return index;
}

static void bin_insert(AllocatorContext* ctx, FreeBlock* block) {
    int idx = get_bin_index(ctx, block->header.size);
    intptr_t block_rel = PTR_TO_REL(ctx, block);
    
    FreeBlock* head = (FreeBlock*)REL_TO_PTR(ctx, ctx->bin_offsets[idx]);
    
    block->rel_next_free = ctx->bin_offsets[idx];
    block->rel_prev_free = -1;
    
    if (head) {
        head->rel_prev_free = block_rel;
    }
    
    ctx->bin_offsets[idx] = block_rel;
    block->header.magic = MAGIC_FREE;
}

static void bin_remove(AllocatorContext* ctx, FreeBlock* block) {
    int idx = get_bin_index(ctx, block->header.size);
    
    FreeBlock* prev = (FreeBlock*)REL_TO_PTR(ctx, block->rel_prev_free);
    FreeBlock* next = (FreeBlock*)REL_TO_PTR(ctx, block->rel_next_free);
    
    if (prev) {
        prev->rel_next_free = block->rel_next_free;
    } else {
        ctx->bin_offsets[idx] = block->rel_next_free;
    }
    
    if (next) {
        next->rel_prev_free = block->rel_prev_free;
    }
    
    block->rel_next_free = -1;
    block->rel_prev_free = -1;
}

/* --- Best-Fit Strategy (Red-Black Tree) --- */
/* Simplified best-fit using linear search for now - full RB-tree is complex */

static FreeBlock* find_best_fit(AllocatorContext* ctx, size_t size) {
    FreeBlock* best = NULL;
    size_t best_size = SIZE_MAX;
    
    /* Search all bins that could contain a suitable block */
    for (uint32_t i = 0; i < ctx->settings.bin_count; i++) {
        FreeBlock* curr = (FreeBlock*)REL_TO_PTR(ctx, ctx->bin_offsets[i]);
        
        while (curr) {
            if (curr->header.size >= size && curr->header.size < best_size) {
                best = curr;
                best_size = curr->header.size;
                
                /* Perfect fit - stop searching */
                if (best_size == size) {
                    return best;
                }
            }
            curr = (FreeBlock*)REL_TO_PTR(ctx, curr->rel_next_free);
        }
    }
    
    return best;
}

/* --- Coalescing --- */

static void coalesce_block(AllocatorContext* ctx, BlockHeader* h) {
    /* Coalesce with next block */
    if (h->rel_next_phys != -1) {
        BlockHeader* next = (BlockHeader*)((uint8_t*)h + h->rel_next_phys);
        
        if (next->is_free && validate_block_internal(ctx, next, false)) {
            bin_remove(ctx, (FreeBlock*)next);
            
            h->size += next->size;
            h->rel_next_phys = next->rel_next_phys;
            
            /* Update the block after next to point back to h */
            if (next->rel_next_phys != -1) {
                BlockHeader* next_next = (BlockHeader*)((uint8_t*)next + next->rel_next_phys);
                next_next->rel_prev_phys = PTR_TO_REL(next_next, h);
            }
            
            ctx->telemetry.coalescingEvents++;
        }
    }
    
    /* Coalesce with previous block */
    if (h->rel_prev_phys != -1) {
        BlockHeader* prev = (BlockHeader*)((uint8_t*)h + h->rel_prev_phys);
        
        if (prev->is_free && validate_block_internal(ctx, prev, false)) {
            bin_remove(ctx, (FreeBlock*)prev);
            
            prev->size += h->size;
            prev->rel_next_phys = h->rel_next_phys;
            
            /* Update the block after h to point back to prev */
            if (h->rel_next_phys != -1) {
                BlockHeader* next = (BlockHeader*)((uint8_t*)h + h->rel_next_phys);
                next->rel_prev_phys = PTR_TO_REL(next, prev);
            }
            
            /* Now work with prev as the coalesced block */
            h = prev;
            ctx->telemetry.coalescingEvents++;
        }
    }
    
    /* Re-insert the coalesced block */
    bin_insert(ctx, (FreeBlock*)h);
}

/* --- Quarantine Management --- */

static void quarantine_push(AllocatorContext* ctx, void* ptr, size_t size) {
    if (!ctx->settings.useQuarantine) return;
    
    /* Flush oldest if over limit */
    while (ctx->quarantine.currentBytes + size > ctx->quarantine.maxBytes && 
           ctx->quarantine.head) {
        QuarantineNode* node = ctx->quarantine.head;
        ctx->quarantine.head = node->next;
        ctx->quarantine.currentBytes -= node->size;
        
        BlockHeader* h = (BlockHeader*)((uint8_t*)node->ptr - sizeof(BlockHeader));
        h->is_free = 1;
        h->magic = MAGIC_FREE;
        coalesce_block(ctx, h);
        
        ctx->telemetry.quarantinedBlocks--;
    }
    
    /* Add to quarantine (simplified - would need proper memory pool) */
    ctx->quarantine.currentBytes += size;
    ctx->telemetry.quarantinedBlocks++;
    ctx->telemetry.quarantinedBytes = ctx->quarantine.currentBytes;
}

/* --- Heap Growth & Relocation --- */

static bool grow_heap(AllocatorContext** ctx_ref, size_t needed) {
    AllocatorContext* ctx = *ctx_ref;
    
    if (ctx->managed) {
        set_error(ctx, ERR_HEAP_LIMIT_REACHED,
                  "Cannot grow managed heap (fixed size: %llu bytes)",
                  (unsigned long long)ctx->total_size);
        return false;
    }

    size_t pg_size = getPageSize();
    size_t req_size = ALIGN_UP(needed, pg_size);
    
    if (ctx->settings.maxHeapSize > 0 && 
        (ctx->total_size + req_size > ctx->settings.maxHeapSize)) {
        set_error(ctx, ERR_HEAP_LIMIT_REACHED,
                  "Allocation would exceed maxHeapSize (%llu + %llu > %llu)",
                  (unsigned long long)ctx->total_size,
                  (unsigned long long)req_size,
                  (unsigned long long)ctx->settings.maxHeapSize);
        return false;
    }

    if (ctx->telemetry.metadataPageCount + 1 > ctx->settings.maxMetadataPages) {
        set_error(ctx, ERR_METADATA_CAP_EXCEEDED,
                  "Would exceed maxMetadataPages limit (%llu)",
                  (unsigned long long)ctx->settings.maxMetadataPages);
        return false;
    }

    void* new_mem = allocPages(req_size);
    if (!new_mem) {
        set_error(ctx, ERR_OUT_OF_MEMORY,
                  "System allocPages() failed for %llu bytes",
                  (unsigned long long)req_size);
        return false;
    }

    bool contiguous = (new_mem == (uint8_t*)ctx->base_addr + ctx->total_size);

    if (ctx->settings.ensureContiguous && !contiguous) {
        size_t new_total = ctx->total_size + req_size;
        void* big_strip = allocPages(new_total);
        
        if (!big_strip) {
            freePages(new_mem, req_size);
            set_error(ctx, ERR_RELOCATION_FAILED,
                      "Cannot allocate contiguous block of %llu bytes",
                      (unsigned long long)new_total);
            return false;
        }

        memcpy(big_strip, ctx->base_addr, ctx->total_size);
        freePages(ctx->base_addr, ctx->total_size);
        freePages(new_mem, req_size);

        ctx = (AllocatorContext*)big_strip;
        ctx->base_addr = big_strip;
        ctx->total_size = new_total;
        *ctx_ref = ctx;
    } else {
        ctx->total_size += req_size;
    }

    /* Create new free block */
    BlockHeader* new_h = (BlockHeader*)((uint8_t*)ctx->base_addr + 
                                        (ctx->total_size - req_size));
    new_h->magic = MAGIC_FREE;
    new_h->canary_top = get_canary(ctx);
    new_h->size = req_size;
    new_h->requested_size = 0;
    new_h->is_free = 1;
    new_h->rel_next_phys = -1;
    new_h->rel_prev_phys = -1;
    new_h->alloc_file = NULL;
    new_h->alloc_line = 0;
    new_h->alloc_timestamp = 0;
    
    /* Link to existing heap if contiguous */
    if (contiguous) {
        /* Find the last block in the existing heap */
        BlockHeader* curr = (BlockHeader*)((uint8_t*)ctx->base_addr + 
                                           sizeof(AllocatorContext) + 
                                           sizeof(intptr_t) * ctx->settings.bin_count);
        
        while (curr->rel_next_phys != -1) {
            curr = (BlockHeader*)((uint8_t*)curr + curr->rel_next_phys);
        }
        
        curr->rel_next_phys = PTR_TO_REL(curr, new_h);
        new_h->rel_prev_phys = PTR_TO_REL(new_h, curr);
        
        /* Try to coalesce with last block if it's free */
        if (curr->is_free) {
            bin_remove(ctx, (FreeBlock*)curr);
            curr->size += new_h->size;
            curr->rel_next_phys = -1;
            new_h = curr;
        }
    }
    
    bin_insert(ctx, (FreeBlock*)new_h);
    ctx->telemetry.metadataPageCount = (ctx->total_size + pg_size - 1) / pg_size;
    ctx->telemetry.heapGrowthEvents++;
    
    return true;
}

/* --- Allocation Implementation --- */

static void* allocate_block(AllocatorContext** ctx_ref, size_t size, 
                           const char* file, int line) {
    AllocatorContext* ctx = *ctx_ref;
    
    /* Validation */
    if (size == 0) {
        set_error(ctx, ERR_INVALID_SIZE, "Allocation size is 0");
        return NULL;
    }
    
    /* Calculate overhead */
    size_t overhead = sizeof(BlockHeader);
    if (ctx->settings.useCanaries) {
        overhead += sizeof(uint32_t); /* Bottom canary */
    }
    
    /* Check for overflow */
    if (size > SIZE_MAX - overhead) {
        set_error(ctx, ERR_SIZE_OVERFLOW,
                  "Requested size %llu + overhead %llu exceeds maximum",
                  (unsigned long long)size, (unsigned long long)overhead);
        return NULL;
    }
    
    size_t actual = ALIGN_UP(size + overhead, ctx->settings.alignment);
    
    /* Enforce minimum block size */
    if (actual < MIN_BLOCK_SIZE) {
        actual = MIN_BLOCK_SIZE;
    }
    
    /* Large allocation threshold - direct mmap */
    if (ctx->settings.largeMmapThreshold > 0 && 
        size >= ctx->settings.largeMmapThreshold) {
        /* Simplified - would need separate tracking */
        ctx->telemetry.largeAllocations++;
    }
    
    FreeBlock* chosen = NULL;
    
retry:
    /* Find suitable block based on strategy */
    if (ctx->settings.strategy == STRATEGY_BEST_FIT) {
        chosen = find_best_fit(ctx, actual);
    } else {
        /* Segregated fit or first fit */
        for (uint32_t i = get_bin_index(ctx, actual); i < ctx->settings.bin_count; i++) {
            FreeBlock* curr = (FreeBlock*)REL_TO_PTR(ctx, ctx->bin_offsets[i]);
            
            while (curr) {
                if (ctx->settings.validateOnEntry && !validate_block(ctx, &curr->header)) {
                    return NULL;
                }

                if (curr->header.size >= actual) {
                    chosen = curr;
                    break;
                }
                
                curr = (FreeBlock*)REL_TO_PTR(ctx, curr->rel_next_free);
            }
            
            if (chosen) break;
        }
    }
    
    if (!chosen) {
        if (grow_heap(ctx_ref, actual)) {
            ctx = *ctx_ref;
            goto retry;
        }
        return NULL;
    }
    
    /* Remove from free list */
    bin_remove(ctx, chosen);
    
    /* Split block if remainder is large enough */
    if (chosen->header.size - actual >= ctx->settings.min_split_threshold) {
        FreeBlock* remainder = (FreeBlock*)((uint8_t*)chosen + actual);
        
        remainder->header.magic = MAGIC_FREE;
        remainder->header.canary_top = get_canary(ctx);
        remainder->header.size = chosen->header.size - actual;
        remainder->header.requested_size = 0;
        remainder->header.is_free = 1;
        remainder->header.rel_next_phys = chosen->header.rel_next_phys;
        remainder->header.rel_prev_phys = PTR_TO_REL(remainder, chosen);
        remainder->header.alloc_file = NULL;
        remainder->header.alloc_line = 0;
        remainder->header.alloc_timestamp = 0;
        
        /* Update chosen block */
        chosen->header.rel_next_phys = PTR_TO_REL(chosen, remainder);
        chosen->header.size = actual;
        
        /* Update next block's prev pointer */
        if (remainder->header.rel_next_phys != -1) {
            BlockHeader* next = (BlockHeader*)((uint8_t*)remainder + 
                                               remainder->header.rel_next_phys);
            next->rel_prev_phys = PTR_TO_REL(next, remainder);
        }
        
        bin_insert(ctx, remainder);
        ctx->telemetry.splittingEvents++;
    }
    
    /* Finalize allocated block */
    chosen->header.magic = MAGIC_ALLOCATED;
    chosen->header.is_free = 0;
    chosen->header.requested_size = size;
    chosen->header.alloc_timestamp = ++ctx->alloc_counter;
    
    if (ctx->settings.trackCallSites) {
        chosen->header.alloc_file = file;
        chosen->header.alloc_line = line;
    }
    
    if (ctx->settings.useCanaries) {
        uint32_t* bot_canary = (uint32_t*)((uint8_t*)chosen + chosen->header.size - sizeof(uint32_t));
        *bot_canary = chosen->header.canary_top;
    }
    
    /* Update telemetry */
    if (ctx->settings.enableTelemetry) {
        ctx->telemetry.activeAllocations++;
        ctx->telemetry.allocationCount++;
        ctx->telemetry.totalAllocated += size;
        ctx->telemetry.totalOverhead += (actual - size);
        
        if (ctx->telemetry.activeAllocations > ctx->telemetry.peakActiveAllocations) {
            ctx->telemetry.peakActiveAllocations = ctx->telemetry.activeAllocations;
        }
        
        len_t current_usage = ctx->telemetry.totalAllocated + ctx->telemetry.totalOverhead;
        if (current_usage > ctx->telemetry.peakUsage) {
            ctx->telemetry.peakUsage = current_usage;
        }
    }
    
    return (void*)((uint8_t*)chosen + sizeof(BlockHeader));
}

/* --- Deallocation Implementation --- */

static void free_block(AllocatorContext* ctx, void* ptr) {
    if (!ptr) return;
    
    BlockHeader* h = (BlockHeader*)((uint8_t*)ptr - sizeof(BlockHeader));
    
    /* Validate block */
    if (!validate_block(ctx, h)) {
        return; /* Error already set */
    }
    
    if (h->magic != MAGIC_ALLOCATED) {
        set_error(ctx, ERR_INVALID_POINTER,
                  "Pointer %p has invalid magic (not from this allocator or corrupted)", ptr);
        return;
    }
    
    if (h->is_free) {
        set_error(ctx, ERR_DOUBLE_FREE,
                  "Double free detected at %p (allocated at %s:%d, timestamp %llu)",
                  ptr, h->alloc_file ? h->alloc_file : "unknown",
                  h->alloc_line, (unsigned long long)h->alloc_timestamp);
        return;
    }
    
    size_t payload_size = h->requested_size;
    
    /* Security features */
    if (ctx->settings.zeroOnFree && !ctx->settings.poisonOnFree) {
        memset(ptr, 0, payload_size);
    } else if (ctx->settings.poisonOnFree) {
        memset(ptr, 0xDE, payload_size);
    }
    
    /* Update telemetry */
    if (ctx->settings.enableTelemetry) {
        ctx->telemetry.activeAllocations--;
        ctx->telemetry.freeCount++;
        ctx->telemetry.totalAllocated -= payload_size;
        ctx->telemetry.totalOverhead -= (h->size - sizeof(BlockHeader) - payload_size);
    }
    
    /* Quarantine if enabled */
    if (ctx->settings.useQuarantine) {
        quarantine_push(ctx, ptr, h->size);
        return;
    }
    
    /* Mark as free and coalesce */
    h->is_free = 1;
    h->magic = MAGIC_FREE;
    
    if (ctx->settings.useDeferredCoalescing) {
        /* Just add to free list, defer coalescing */
        bin_insert(ctx, (FreeBlock*)h);
    } else {
        /* Immediate coalescing */
        coalesce_block(ctx, h);
    }
}

/* --- Realloc Implementation --- */

static void* realloc_block(AllocatorContext** ctx_ref, void* ptr, size_t new_size,
                          const char* file, const char* module, int line) {
    AllocatorContext* ctx = *ctx_ref;
    
    /* Standard realloc semantics */
    if (!ptr) {
        return allocate_block(ctx_ref, new_size, file, line);
    }
    
    if (new_size == 0) {
        free_block(ctx, ptr);
        return NULL;
    }
    
    BlockHeader* h = (BlockHeader*)((uint8_t*)ptr - sizeof(BlockHeader));
    
    if (!validate_block(ctx, h)) {
        return NULL;
    }
    
    size_t old_size = h->requested_size;
    
    /* If new size fits in current block, just update */
    size_t overhead = sizeof(BlockHeader);
    if (ctx->settings.useCanaries) overhead += sizeof(uint32_t);
    
    size_t needed = ALIGN_UP(new_size + overhead, ctx->settings.alignment);
    if (needed < MIN_BLOCK_SIZE) needed = MIN_BLOCK_SIZE;
    
    if (needed <= h->size) {
        /* Shrinking or fits in current block */
        h->requested_size = new_size;
        
        if (ctx->settings.enableTelemetry) {
            ctx->telemetry.reallocCount++;
        }
        
        return ptr;
    }
    
    /* Try to expand into next block if it's free */
    if (h->rel_next_phys != -1) {
        BlockHeader* next = (BlockHeader*)((uint8_t*)h + h->rel_next_phys);
        
        if (next->is_free && (h->size + next->size >= needed)) {
            bin_remove(ctx, (FreeBlock*)next);
            
            size_t combined = h->size + next->size;
            h->size = combined;
            h->rel_next_phys = next->rel_next_phys;
            
            if (next->rel_next_phys != -1) {
                BlockHeader* next_next = (BlockHeader*)((uint8_t*)next + next->rel_next_phys);
                next_next->rel_prev_phys = PTR_TO_REL(next_next, h);
            }
            
            h->requested_size = new_size;
            
            /* Update bottom canary */
            if (ctx->settings.useCanaries) {
                uint32_t* bot = (uint32_t*)((uint8_t*)h + h->size - sizeof(uint32_t));
                *bot = h->canary_top;
            }
            
            if (ctx->settings.enableTelemetry) {
                ctx->telemetry.reallocCount++;
                ctx->telemetry.coalescingEvents++;
            }
            
            return ptr;
        }
    }
    
    /* Need to allocate new block and copy */
    void* new_ptr = allocate_block(ctx_ref, new_size, file, line);
    if (!new_ptr) {
        set_error(*ctx_ref, ERR_REALLOC_FAILED,
                  "Failed to allocate %llu bytes for realloc",
                  (unsigned long long)new_size);
        return NULL;
    }
    
    memcpy(new_ptr, ptr, MIN(old_size, new_size));
    free_block(*ctx_ref, ptr);
    
    if ((*ctx_ref)->settings.enableTelemetry) {
        (*ctx_ref)->telemetry.reallocCount++;
    }
    
    return new_ptr;
}

/* --- Calloc Implementation --- */

static void* calloc_block(AllocatorContext** ctx_ref, size_t nmemb, size_t size,
                         const char* file, int line) {
    /* Check for overflow */
    if (nmemb != 0 && size > SIZE_MAX / nmemb) {
        set_error(*ctx_ref, ERR_SIZE_OVERFLOW,
                  "calloc overflow: %llu * %llu exceeds maximum",
                  (unsigned long long)nmemb, (unsigned long long)size);
        return NULL;
    }
    
    size_t total = nmemb * size;
    void* ptr = allocate_block(ctx_ref, total, file, line);
    
    if (ptr) {
        memset(ptr, 0, total);
    }
    
    return ptr;
}

/* --- Aligned Allocation --- */

static void* aligned_alloc_block(AllocatorContext** ctx_ref, size_t alignment, size_t size,
                                const char* file, int line) {
    AllocatorContext* ctx = *ctx_ref;
    
    /* Validate alignment */
    if (alignment == 0 || !IS_POWER_OF_TWO(alignment)) {
        set_error(ctx, ERR_ALIGNMENT_FAILURE,
                  "Alignment must be power of 2, got %llu",
                  (unsigned long long)alignment);
        return NULL;
    }
    
    if (alignment > getPageSize()) {
        set_error(ctx, ERR_ALIGNMENT_FAILURE,
                  "Alignment %llu exceeds page size %llu",
                  (unsigned long long)alignment, (unsigned long long)getPageSize());
        return NULL;
    }
    
    /* Allocate extra space for alignment */
    size_t extra = alignment + sizeof(BlockHeader);
    void* ptr = allocate_block(ctx_ref, size + extra, file, line);
    
    if (!ptr) return NULL;
    
    /* Find aligned address within allocated block */
    uintptr_t addr = (uintptr_t)ptr;
    uintptr_t aligned = ALIGN_UP(addr, alignment);
    
    /* If already aligned, return as-is */
    if (aligned == addr) {
        return ptr;
    }
    
    /* Store offset for later freeing (simplified - production would need more) */
    return (void*)aligned;
}

/* --- Public API --- */

void* std_malloc_debug(AllocatorContext** ctx_ref, size_t size, 
                       const char* file, int line) {
    return allocate_block(ctx_ref, size, file, line);
}

void* std_malloc(AllocatorContext** ctx_ref, size_t size) {
    return allocate_block(ctx_ref, size, "<unknown>", 0);
}

void* std_calloc(AllocatorContext** ctx_ref, size_t nmemb, size_t size) {
    return calloc_block(ctx_ref, nmemb, size, "<unknown>", 0);
}

void* std_realloc(AllocatorContext** ctx_ref, void* ptr, size_t size) {
    return realloc_block(ctx_ref, ptr, size, "<unknown>", 0);
}

void* std_aligned_alloc(AllocatorContext** ctx_ref, size_t alignment, size_t size) {
    return aligned_alloc_block(ctx_ref, alignment, size, "<unknown>", 0);
}

void std_free(AllocatorContext* ctx, void* ptr) {
    free_block(ctx, ptr);
}

void std_free_diag(AllocatorContext* ctx, void* ptr) {
    free_block(ctx, ptr);
}

/* --- Heap Introspection --- */

void dump_heap_stats(AllocatorContext* ctx, FILE* out) {
    fprintf(out, "=== Allocator Statistics ===\n");
    fprintf(out, "Total allocations: %llu\n", (unsigned long long)ctx->telemetry.allocationCount);
    fprintf(out, "Total frees: %llu\n", (unsigned long long)ctx->telemetry.freeCount);
    fprintf(out, "Active allocations: %llu\n", (unsigned long long)ctx->telemetry.activeAllocations);
    fprintf(out, "Peak active: %llu\n", (unsigned long long)ctx->telemetry.peakActiveAllocations);
    fprintf(out, "Total allocated: %llu bytes\n", (unsigned long long)ctx->telemetry.totalAllocated);
    fprintf(out, "Total overhead: %llu bytes\n", (unsigned long long)ctx->telemetry.totalOverhead);
    fprintf(out, "Peak usage: %llu bytes\n", (unsigned long long)ctx->telemetry.peakUsage);
    fprintf(out, "Heap size: %llu bytes\n", (unsigned long long)ctx->total_size);
    fprintf(out, "Fragmentation: %llu bytes\n", (unsigned long long)ctx->telemetry.fragmentationBytes);
    fprintf(out, "Coalescing events: %llu\n", (unsigned long long)ctx->telemetry.coalescingEvents);
    fprintf(out, "Splitting events: %llu\n", (unsigned long long)ctx->telemetry.splittingEvents);
    fprintf(out, "Heap growths: %llu\n", (unsigned long long)ctx->telemetry.heapGrowthEvents);
    fprintf(out, "Corruption events: %llu\n", (unsigned long long)ctx->telemetry.corruptionEvents);
    
    if (ctx->settings.useQuarantine) {
        fprintf(out, "Quarantined blocks: %llu\n", (unsigned long long)ctx->telemetry.quarantinedBlocks);
        fprintf(out, "Quarantined bytes: %llu\n", (unsigned long long)ctx->telemetry.quarantinedBytes);
    }
    
    fprintf(out, "Last error: %s\n", ctx->telemetry.lastErrorMsg);
    fprintf(out, "===========================\n");
}

void dump_allocations(AllocatorContext* ctx, FILE* out) {
    fprintf(out, "=== Active Allocations ===\n");
    
    BlockHeader* curr = (BlockHeader*)((uint8_t*)ctx->base_addr + 
                                       sizeof(AllocatorContext) + 
                                       sizeof(intptr_t) * ctx->settings.bin_count);
    
    uint64_t count = 0;
    while (curr) {
        if (!curr->is_free) {
            count++;
            fprintf(out, "[%llu] %p: %llu bytes", count, 
                   (void*)((uint8_t*)curr + sizeof(BlockHeader)),
                   (unsigned long long)curr->requested_size);
            
            if (ctx->settings.trackCallSites && curr->alloc_file) {
                fprintf(out, " (allocated at %s:%d, ts=%llu)",
                       curr->alloc_file, curr->alloc_line,
                       (unsigned long long)curr->alloc_timestamp);
            }
            fprintf(out, "\n");
        }
        
        if (curr->rel_next_phys == -1) break;
        curr = (BlockHeader*)((uint8_t*)curr + curr->rel_next_phys);
    }
    
    fprintf(out, "Total active: %llu\n", count);
    fprintf(out, "==========================\n");
}

/* --- Initialization --- */

static MemAllocSettings get_default_settings(OptimizationMode mode) {
    MemAllocSettings s = {0};
    
    /* Common defaults */
    s.mode = mode;
    s.bin_count = 32;
    s.alignment = 16;
    s.maxMetadataPages = 1000;
    s.ensureContiguous = false;
    s.allowRuntimeTuning = true;
    s.enableTelemetry = true;
    s.maxHeapSize = 0; /* Unlimited */
    s.verboseErrors = true;
    s.errorCallback = NULL;
    
    switch (mode) {
        case OPTIMIZE_SPEED:
            s.strategy = STRATEGY_SEGREGATED_FIT;
            s.min_split_threshold = 64;
            s.zeroOnFree = false;
            s.poisonOnFree = false;
            s.useCanaries = false;
            s.validateOnEntry = false;
            s.useRandomCanaries = false;
            s.useQuarantine = false;
            s.useDeferredCoalescing = true;
            s.trackCallSites = false;
            s.largeMmapThreshold = 128 * 1024;
            break;
            
        case OPTIMIZE_SPACE:
            s.strategy = STRATEGY_BEST_FIT;
            s.min_split_threshold = 32;
            s.zeroOnFree = false;
            s.poisonOnFree = false;
            s.useCanaries = false;
            s.validateOnEntry = false;
            s.useRandomCanaries = false;
            s.useQuarantine = false;
            s.useDeferredCoalescing = false;
            s.trackCallSites = false;
            s.largeMmapThreshold = 256 * 1024;
            break;
            
        case OPTIMIZE_BALANCED:
            s.strategy = STRATEGY_SEGREGATED_FIT;
            s.min_split_threshold = 48;
            s.zeroOnFree = false;
            s.poisonOnFree = false;
            s.useCanaries = true;
            s.validateOnEntry = false;
            s.useRandomCanaries = false;
            s.useQuarantine = false;
            s.useDeferredCoalescing = false;
            s.trackCallSites = true;
            s.largeMmapThreshold = 192 * 1024;
            break;
            
        case OPTIMIZE_SECURITY:
            s.strategy = STRATEGY_SEGREGATED_FIT;
            s.min_split_threshold = 64;
            s.zeroOnFree = true;
            s.poisonOnFree = false;
            s.useCanaries = true;
            s.validateOnEntry = true;
            s.useRandomCanaries = true;
            s.useQuarantine = true;
            s.quarantineSize = 1024 * 1024;
            s.useDeferredCoalescing = false;
            s.trackCallSites = true;
            s.largeMmapThreshold = 256 * 1024;
            break;
    }
    
    return s;
}

AllocatorContext* initAllocator(void* base, size_t size, MemAllocSettings settings) {
    /* Validate base alignment */
    if (((uintptr_t)base & (settings.alignment - 1)) != 0) {
        return NULL; /* Can't set error without context */
    }
    
    /* Validate settings */
    if (settings.bin_count == 0 || settings.bin_count > 64) {
        return NULL;
    }
    
    if (!IS_POWER_OF_TWO(settings.alignment) || settings.alignment < 8) {
        return NULL;
    }
    
    if (settings.min_split_threshold < MIN_BLOCK_SIZE) {
        settings.min_split_threshold = MIN_BLOCK_SIZE;
    }
    
    size_t ctx_sz = ALIGN_UP(sizeof(AllocatorContext) + 
                             (sizeof(intptr_t) * settings.bin_count),
                             settings.alignment);
    
    if (ctx_sz >= size) {
        return NULL;
    }
    
    AllocatorContext* ctx = (AllocatorContext*)base;
    memset(ctx, 0, ctx_sz);
    
    ctx->base_addr = base;
    ctx->total_size = size;
    ctx->settings = settings;
    ctx->managed = true;
    ctx->alloc_counter = 0;
    ctx->canary_seed = 0xDEADBEEF; /* Would use better seed in production */
    ctx->rb_tree_root = -1;
    ctx->deferred_coalesce_list = -1;
    
    /* Initialize quarantine */
    if (settings.useQuarantine) {
        ctx->quarantine.head = NULL;
        ctx->quarantine.currentBytes = 0;
        ctx->quarantine.maxBytes = settings.quarantineSize;
    }
    
    /* Initialize bins */
    for (uint32_t i = 0; i < settings.bin_count; i++) {
        ctx->bin_offsets[i] = -1;
    }
    
    /* Create initial free block */
    uint8_t* usable = (uint8_t*)base + ctx_sz;
    BlockHeader* initial = (BlockHeader*)usable;
    
    initial->magic = MAGIC_FREE;
    initial->canary_top = get_canary(ctx);
    initial->size = size - ctx_sz;
    initial->requested_size = 0;
    initial->is_free = 1;
    initial->rel_next_phys = -1;
    initial->rel_prev_phys = -1;
    initial->alloc_file = NULL;
    initial->alloc_line = 0;
    initial->alloc_timestamp = 0;
    
    bin_insert(ctx, (FreeBlock*)initial);
    
    ctx->telemetry.metadataPageCount = (size + getPageSize() - 1) / getPageSize();
    ctx->telemetry.totalMetadataBytes = ctx_sz;
    
    return ctx;
}

AllocatorContext* createAllocator(OptimizationMode mode, size_t initial_size) {
    MemAllocSettings settings = get_default_settings(mode);
    
    size_t pg_size = getPageSize();
    size_t alloc_size = ALIGN_UP(initial_size, pg_size);
    
    void* mem = allocPages(alloc_size);
    if (!mem) return NULL;
    
    AllocatorContext* ctx = initAllocator(mem, alloc_size, settings);
    if (ctx) {
        ctx->managed = false; /* Allow growth */
    }
    
    return ctx;
}

void destroyAllocator(AllocatorContext* ctx) {
    if (!ctx) return;
    
    if (!ctx->managed) {
        freePages(ctx->base_addr, ctx->total_size);
    }
}

/* --- Convenience Macros --- */

#define MALLOC(ctx, size) std_malloc_debug(&(ctx), (size), __FILE__, __LINE__)
#define FREE(ctx, ptr) std_free((ctx), (ptr))
#define CALLOC(ctx, n, size) std_calloc(&(ctx), (n), (size))
#define REALLOC(ctx, ptr, size) std_realloc(&(ctx), (ptr), (size))

/* --- Error Query API --- */

const char* getAllocatorError(AllocatorContext* ctx) {
    return ctx->telemetry.lastErrorMsg;
}

AllocError getLastErrorCode(AllocatorContext* ctx) {
    return ctx->telemetry.lastError;
}

void clearError(AllocatorContext* ctx) {
    ctx->telemetry.lastError = ERR_NONE;
    ctx->telemetry.lastErrorMsg[0] = '\0';
}

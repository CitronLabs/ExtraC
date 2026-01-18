#include "Alloc.h"

/* === ERROR HANDLING === */

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
    [ERR_RUNTIME_TUNING_DISABLED] = "Runtime tuning disabled: cannot change settings",
    [ERR_INCOMPATIBLE_SETTINGS] = "Incompatible settings: cannot apply requested configuration",
    [ERR_LARGE_ALLOC_FAILED] = "Large allocation failed: direct page allocation error"
};

void set_error(AllocatorContext* ctx, AllocError err, const char* fmt, ...) {
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
        ctx->settings.errorCallback(err, ctx->telemetry.lastErrorMsg, 
                                    ctx->settings.callbackUserdata);
    }
}

/* === RANDOM NUMBER GENERATION === */

uint32_t xorshift32(uint32_t* state) {
    uint32_t x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

uint32_t get_canary(AllocatorContext* ctx) {
    if (ctx->settings.useRandomCanaries) {
        return xorshift32(&ctx->canary_seed);
    }
    return CANARY_VALUE_BASE;
}

/* === BLOCK VALIDATION === */

bool validate_block_internal(AllocatorContext* ctx, BlockHeader* h, bool check_canary) {
    /* Check magic number */
    if (h->magic != MAGIC_ALLOCATED && h->magic != MAGIC_FREE) {
        set_error(ctx, ERR_CORRUPTION_DETECTED, 
                  "Invalid magic number at %p: expected 0x%X or 0x%X, got 0x%X",
                  (void*)h, MAGIC_ALLOCATED, MAGIC_FREE, h->magic);
        ctx->telemetry.validationFailures++;
        return false;
    }
    
    /* Check size sanity */
    if (h->size < sizeof(BlockHeader) || h->size > ctx->total_size) {
        set_error(ctx, ERR_CORRUPTION_DETECTED,
                  "Invalid block size at %p: %llu bytes (total heap: %llu)",
                  (void*)h, (unsigned long long)h->size, 
                  (unsigned long long)ctx->total_size);
        ctx->telemetry.validationFailures++;
        return false;
    }
    
    /* Check canaries if enabled */
    if (check_canary && ctx->settings.useCanaries && !h->is_free) {
        uint32_t* canary_bot = (uint32_t*)((uint8_t*)h + h->size - sizeof(uint32_t));
        if (*canary_bot != h->canary_top) {
            set_error(ctx, ERR_CORRUPTION_DETECTED,
                      "Bottom canary corrupted at %p: expected 0x%X, got 0x%X (buffer overflow)",
                      (void*)h, h->canary_top, *canary_bot);
            ctx->telemetry.validationFailures++;
            return false;
        }
    }
    
    return true;
}

bool validate_block(AllocatorContext* ctx, BlockHeader* h) {
    return validate_block_internal(ctx, h, true);
}

/* === BIN MANAGEMENT === */

int get_bin_index(AllocatorContext* ctx, size_t size) {
    if (size <= ctx->settings.alignment) return 0;
    
    /* Calculate log2 of size */
    int bit = 0;
    size_t temp = size - 1;
    while (temp > 0) {
        bit++;
        temp >>= 1;
    }
    
    /* Map to bin index (bins represent power-of-2 size classes) */
    int index = bit - 4;  /* Bins start at size 16 (2^4) */
    if (index < 0) index = 0;
    if (index >= (int)ctx->settings.bin_count) {
        index = ctx->settings.bin_count - 1;
    }
    
    return index;
}

void bin_insert(AllocatorContext* ctx, FreeBlock* block) {
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

void bin_remove(AllocatorContext* ctx, FreeBlock* block) {
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

/* === ALLOCATION STRATEGIES === */

FreeBlock* find_segregated_fit(AllocatorContext* ctx, size_t size) {
    /* Search bins starting from appropriate size class */
    for (uint32_t i = get_bin_index(ctx, size); i < ctx->settings.bin_count; i++) {
        FreeBlock* curr = (FreeBlock*)REL_TO_PTR(ctx, ctx->bin_offsets[i]);
        
        while (curr) {
            if (ctx->settings.validateOnEntry && !validate_block(ctx, &curr->header)) {
                return NULL;
            }
            
            if (curr->header.size >= size) {
                return curr;
            }
            
            curr = (FreeBlock*)REL_TO_PTR(ctx, curr->rel_next_free);
        }
    }
    
    return NULL;
}

FreeBlock* find_best_fit(AllocatorContext* ctx, size_t size) {
    FreeBlock* best = NULL;
    size_t best_size = SIZE_MAX;
    
    /* Search all bins for the best fitting block */
    for (uint32_t i = 0; i < ctx->settings.bin_count; i++) {
        FreeBlock* curr = (FreeBlock*)REL_TO_PTR(ctx, ctx->bin_offsets[i]);
        
        while (curr) {
            if (curr->header.size >= size && curr->header.size < best_size) {
                best = curr;
                best_size = curr->header.size;
                
                /* Perfect fit - can't do better */
                if (best_size == size) {
                    return best;
                }
            }
            
            curr = (FreeBlock*)REL_TO_PTR(ctx, curr->rel_next_free);
        }
    }
    
    return best;
}

FreeBlock* find_first_fit(AllocatorContext* ctx, size_t size) {
    /* Search all bins in order, return first suitable block */
    for (uint32_t i = 0; i < ctx->settings.bin_count; i++) {
        FreeBlock* curr = (FreeBlock*)REL_TO_PTR(ctx, ctx->bin_offsets[i]);
        
        while (curr) {
            if (curr->header.size >= size) {
                return curr;
            }
            
            curr = (FreeBlock*)REL_TO_PTR(ctx, curr->rel_next_free);
        }
    }
    
    return NULL;
}

/* === COALESCING === */

void coalesce_block(AllocatorContext* ctx, BlockHeader* h) {
    /* Coalesce with next physical block if it's free */
    if (h->rel_next_phys != -1) {
        BlockHeader* next = (BlockHeader*)((uint8_t*)h + h->rel_next_phys);
        
        if (next->is_free && validate_block_internal(ctx, next, false)) {
            /* Remove next from free list */
            bin_remove(ctx, (FreeBlock*)next);
            
            /* Merge sizes */
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
    
    /* Coalesce with previous physical block if it's free */
    if (h->rel_prev_phys != -1) {
        BlockHeader* prev = (BlockHeader*)((uint8_t*)h + h->rel_prev_phys);
        
        if (prev->is_free && validate_block_internal(ctx, prev, false)) {
            /* Remove prev from free list */
            bin_remove(ctx, (FreeBlock*)prev);
            
            /* Merge sizes */
            prev->size += h->size;
            prev->rel_next_phys = h->rel_next_phys;
            
            /* Update the block after h to point back to prev */
            if (h->rel_next_phys != -1) {
                BlockHeader* next = (BlockHeader*)((uint8_t*)h + h->rel_next_phys);
                next->rel_prev_phys = PTR_TO_REL(next, prev);
            }
            
            /* Continue with prev as the merged block */
            h = prev;
            ctx->telemetry.coalescingEvents++;
        }
    }
    
    /* Insert the (potentially coalesced) block back into free list */
    bin_insert(ctx, (FreeBlock*)h);
}

/* === DEFERRED COALESCING === */

void process_deferred_coalescing(AllocatorContext* ctx) {
    if (!ctx->deferred_list) return;
    
    DeferredBlock* curr = ctx->deferred_list;
    
    while (curr) {
        BlockHeader* h = curr->block;
        
        if (h->is_free) {
            /* Remove from free list and coalesce */
            bin_remove(ctx, (FreeBlock*)h);
            coalesce_block(ctx, h);
        }
        
        curr = curr->next;
    }
    
    /* Clear deferred list */
    ctx->deferred_list = NULL;
    ctx->deferred_count = 0;
    ctx->telemetry.deferredCoalescingRuns++;
}

void add_to_deferred_list(AllocatorContext* ctx, BlockHeader* h) {
    /* Use space within the block header for deferred list node */
    DeferredBlock* node = (DeferredBlock*)((uint8_t*)h + sizeof(BlockHeader));
    node->block = h;
    node->next = ctx->deferred_list;
    ctx->deferred_list = node;
    ctx->deferred_count++;
    
    /* Still add to free list immediately */
    bin_insert(ctx, (FreeBlock*)h);
    
    /* Process deferred list if threshold reached */
    if (ctx->deferred_count >= ctx->deferred_threshold) {
        process_deferred_coalescing(ctx);
    }
}


/* === QUARANTINE MANAGEMENT === */

void quarantine_release_oldest(AllocatorContext* ctx) {
    if (!ctx->quarantine.head) return;
    
    QuarantineNode* node = ctx->quarantine.head;
    ctx->quarantine.head = node->next;
    
    if (!ctx->quarantine.head) {
        ctx->quarantine.tail = NULL;
    }
    
    BlockHeader* h = (BlockHeader*)node->block_ptr;
    ctx->quarantine.currentBytes -= h->size;
    ctx->quarantine.nodeCount--;
    ctx->telemetry.quarantinedBlocks--;
    
    /* Actually free the block now */
    h->is_free = 1;
    h->magic = MAGIC_FREE;
    
    if (ctx->settings.useDeferredCoalescing) {
        add_to_deferred_list(ctx, h);
    } else {
        coalesce_block(ctx, h);
    }
}

void quarantine_push(AllocatorContext* ctx, BlockHeader* h) {
    if (!ctx->settings.useQuarantine) {
        /* Quarantine disabled - free immediately */
        h->is_free = 1;
        h->magic = MAGIC_FREE;
        
        if (ctx->settings.useDeferredCoalescing) {
            add_to_deferred_list(ctx, h);
        } else {
            coalesce_block(ctx, h);
        }
        return;
    }
    
    /* Release oldest blocks if over capacity */
    while (ctx->quarantine.currentBytes + h->size > ctx->quarantine.maxBytes) {
        quarantine_release_oldest(ctx);
    }
    
    /* Create quarantine node within the block itself */
    QuarantineNode* node = (QuarantineNode*)((uint8_t*)h + sizeof(BlockHeader));
    node->block_ptr = h;
    node->size = h->size;
    node->timestamp = ctx->alloc_counter;
    node->next = NULL;
    
    /* Add to tail of quarantine queue */
    if (ctx->quarantine.tail) {
        ctx->quarantine.tail->next = node;
    } else {
        ctx->quarantine.head = node;
    }
    ctx->quarantine.tail = node;
    
    /* Update quarantine stats */
    ctx->quarantine.currentBytes += h->size;
    ctx->quarantine.nodeCount++;
    ctx->telemetry.quarantinedBlocks++;
    ctx->telemetry.quarantinedBytes = ctx->quarantine.currentBytes;
}

void quarantine_flush(AllocatorContext* ctx) {
    while (ctx->quarantine.head) {
        quarantine_release_oldest(ctx);
    }
}

/* === LARGE ALLOCATION TRACKING === */

void track_large_alloc(AllocatorContext* ctx, void* ptr, size_t size, 
                       size_t requested, const char* file, int line) {
    /* Allocate tracking structure */
    LargeAlloc* la = (LargeAlloc*)allocPages(sizeof(LargeAlloc));
    if (!la) return;  /* Out of memory - continue without tracking */
    
    la->ptr = ptr;
    la->size = size;
    la->requested_size = requested;
    la->alloc_file = file;
    la->alloc_line = line;
    la->alloc_timestamp = ++ctx->alloc_counter;
    
    /* Add to head of list */
    la->next = ctx->large_allocs;
    ctx->large_allocs = la;
    
    ctx->telemetry.largeAllocations++;
}

bool untrack_large_alloc(AllocatorContext* ctx, void* ptr) {
    LargeAlloc** curr = &ctx->large_allocs;
    
    while (*curr) {
        if ((*curr)->ptr == ptr) {
            LargeAlloc* to_free = *curr;
            size_t size = to_free->size;
            
            /* Remove from list */
            *curr = (*curr)->next;
            
            /* Free tracking structure and memory */
            freePages(ptr, size);
            freePages(to_free, sizeof(LargeAlloc));
            
            ctx->telemetry.largeAllocations--;
            return true;
        }
        curr = &(*curr)->next;
    }
    
    return false;
}

LargeAlloc* find_large_alloc(AllocatorContext* ctx, void* ptr) {
    LargeAlloc* curr = ctx->large_allocs;
    while (curr) {
        if (curr->ptr == ptr) return curr;
        curr = curr->next;
    }
    return NULL;
}

/* === HEAP GROWTH === */

bool grow_heap(AllocatorContext** ctx_ref, size_t needed) {
    AllocatorContext* ctx = *ctx_ref;
    
    /* Cannot grow managed (fixed-size) heaps */
    if (ctx->managed) {
        set_error(ctx, ERR_HEAP_LIMIT_REACHED,
                  "Cannot grow managed heap (fixed size: %llu bytes)",
                  (unsigned long long)ctx->total_size);
        return false;
    }

    size_t pg_size = getPageSize();
    size_t req_size = ALIGN_UP(needed, pg_size);
    
    /* Check heap size limit */
    if (ctx->settings.maxHeapSize > 0 && 
        (ctx->total_size + req_size > ctx->settings.maxHeapSize)) {
        set_error(ctx, ERR_HEAP_LIMIT_REACHED,
                  "Allocation would exceed maxHeapSize (%llu + %llu > %llu)",
                  (unsigned long long)ctx->total_size,
                  (unsigned long long)req_size,
                  (unsigned long long)ctx->settings.maxHeapSize);
        return false;
    }

    /* Check metadata page limit */
    if (ctx->telemetry.metadataPageCount + 1 > ctx->settings.maxMetadataPages) {
        set_error(ctx, ERR_METADATA_CAP_EXCEEDED,
                  "Would exceed maxMetadataPages limit (%llu)",
                  (unsigned long long)ctx->settings.maxMetadataPages);
        return false;
    }

    /* Allocate new pages */
    void* new_mem = allocPages(req_size);
    if (!new_mem) {
        set_error(ctx, ERR_OUT_OF_MEMORY,
                  "System allocPages() failed for %llu bytes",
                  (unsigned long long)req_size);
        return false;
    }

    /* Check if new memory is contiguous */
    bool contiguous = (new_mem == (uint8_t*)ctx->base_addr + ctx->total_size);

    /* Handle contiguous requirement */
    if (ctx->settings.ensureContiguous && !contiguous) {
        /* Need to relocate entire heap to ensure contiguity */
        size_t new_total = ctx->total_size + req_size;
        void* big_strip = allocPages(new_total);
        
        if (!big_strip) {
            freePages(new_mem, req_size);
            set_error(ctx, ERR_RELOCATION_FAILED,
                      "Cannot allocate contiguous block of %llu bytes",
                      (unsigned long long)new_total);
            return false;
        }

        /* Copy existing heap to new location */
        memcpy(big_strip, ctx->base_addr, ctx->total_size);
        
        /* Free old heap and unused new memory */
        freePages(ctx->base_addr, ctx->total_size);
        freePages(new_mem, req_size);

        /* Update context */
        ctx = (AllocatorContext*)big_strip;
        ctx->base_addr = big_strip;
        ctx->total_size = new_total;
        *ctx_ref = ctx;
    } else {
        /* Just expand total size */
        ctx->total_size += req_size;
    }

    /* Create new free block at end of heap */
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
    
    /* If contiguous, link to existing heap and try to coalesce */
    if (contiguous) {
        size_t ctx_sz = ALIGN_UP(sizeof(AllocatorContext) + 
                                 sizeof(intptr_t) * ctx->settings.bin_count,
                                 ctx->settings.alignment);
        BlockHeader* curr = (BlockHeader*)((uint8_t*)ctx->base_addr + ctx_sz);
        
        /* Find last block in heap */
        while (curr->rel_next_phys != -1) {
            curr = (BlockHeader*)((uint8_t*)curr + curr->rel_next_phys);
        }
        
        /* Link new block */
        curr->rel_next_phys = PTR_TO_REL(curr, new_h);
        new_h->rel_prev_phys = PTR_TO_REL(new_h, curr);
        
        /* Coalesce if last block is free */
        if (curr->is_free) {
            bin_remove(ctx, (FreeBlock*)curr);
            curr->size += new_h->size;
            curr->rel_next_phys = -1;
            new_h = curr;
        }
    }
    
    /* Add new block to free list */
    bin_insert(ctx, (FreeBlock*)new_h);
    
    /* Update telemetry */
    ctx->telemetry.metadataPageCount = (ctx->total_size + pg_size - 1) / pg_size;
    ctx->telemetry.heapGrowthEvents++;
    
    return true;
}

/* === DIAGNOSTICS === */

void dump_allocations(AllocatorContext* ctx, FILE* out) {
    fprintf(out, "=== Active Allocations ===\n");
    
    size_t ctx_sz = ALIGN_UP(sizeof(AllocatorContext) + 
                             sizeof(intptr_t) * ctx->settings.bin_count,
                             ctx->settings.alignment);
    BlockHeader* curr = (BlockHeader*)((uint8_t*)ctx->base_addr + ctx_sz);
    
    uint64_t count = 0;
    while (curr) {
        if (!curr->is_free) {
            count++;
            fprintf(out, "[%llu] %p: %llu bytes", 
                   count, 
                   (void*)((uint8_t*)curr + sizeof(BlockHeader)),
                   (unsigned long long)curr->requested_size);
            
            if (ctx->settings.trackCallSites && curr->alloc_file) {
                fprintf(out, " (allocated at %s:%d, timestamp %llu)",
                       curr->alloc_file, curr->alloc_line,
                       (unsigned long long)curr->alloc_timestamp);
            }
            fprintf(out, "\n");
        }
        
        if (curr->rel_next_phys == -1) break;
        curr = (BlockHeader*)((uint8_t*)curr + curr->rel_next_phys);
    }
    
    /* Also print large allocations */
    LargeAlloc* la = ctx->large_allocs;
    while (la) {
        count++;
        fprintf(out, "[%llu] %p: %llu bytes (large allocation)", 
               count, la->ptr, (unsigned long long)la->requested_size);
        if (ctx->settings.trackCallSites && la->alloc_file) {
            fprintf(out, " (allocated at %s:%d, timestamp %llu)",
                   la->alloc_file, la->alloc_line,
                   (unsigned long long)la->alloc_timestamp);
        }
        fprintf(out, "\n");
        la = la->next;
    }
    
    fprintf(out, "Total active: %llu\n", count);
    fprintf(out, "==========================\n");
}

void printDetailedStats(AllocatorContext* ctx, FILE* out) {
    fprintf(out, "=== Detailed Allocator Statistics ===\n");
    fprintf(out, "Current Mode: %s\n", 
            ctx->settings.mode == OPTIMIZE_SPEED ? "SPEED" :
            ctx->settings.mode == OPTIMIZE_SPACE ? "SPACE" :
            ctx->settings.mode == OPTIMIZE_BALANCED ? "BALANCED" : "SECURITY");
    fprintf(out, "Strategy: %s\n",
            ctx->settings.strategy == STRATEGY_SEGREGATED_FIT ? "Segregated-fit" :
            ctx->settings.strategy == STRATEGY_BEST_FIT ? "Best-fit" : "First-fit");
    
    fprintf(out, "\nMemory Usage:\n");
    fprintf(out, "  Total allocated: %llu bytes\n", 
            (unsigned long long)ctx->telemetry.totalAllocated);
    fprintf(out, "  Overhead: %llu bytes\n", 
            (unsigned long long)ctx->telemetry.totalOverhead);
    fprintf(out, "  Heap size: %llu bytes\n", 
            (unsigned long long)ctx->total_size);
    fprintf(out, "  Peak usage: %llu bytes\n", 
            (unsigned long long)ctx->telemetry.peakUsage);
    fprintf(out, "  Utilization: %.2f%%\n", 
            ctx->total_size > 0 ? 
            (100.0 * ctx->telemetry.totalAllocated / ctx->total_size) : 0.0);
    
    fprintf(out, "\nOperations:\n");
    fprintf(out, "  Allocations: %llu\n", 
            (unsigned long long)ctx->telemetry.allocationCount);
    fprintf(out, "  Frees: %llu\n", 
            (unsigned long long)ctx->telemetry.freeCount);
    fprintf(out, "  Reallocs: %llu\n", 
            (unsigned long long)ctx->telemetry.reallocCount);
    fprintf(out, "  Active: %llu\n", 
            (unsigned long long)ctx->telemetry.activeAllocations);
    fprintf(out, "  Peak active: %llu\n", 
            (unsigned long long)ctx->telemetry.peakActiveAllocations);
    
    fprintf(out, "\nOptimizations:\n");
    fprintf(out, "  Coalescing events: %llu\n", 
            (unsigned long long)ctx->telemetry.coalescingEvents);
    fprintf(out, "  Splitting events: %llu\n", 
            (unsigned long long)ctx->telemetry.splittingEvents);
    fprintf(out, "  Heap growths: %llu\n", 
            (unsigned long long)ctx->telemetry.heapGrowthEvents);
    fprintf(out, "  Deferred coalescing runs: %llu\n", 
            (unsigned long long)ctx->telemetry.deferredCoalescingRuns);
    fprintf(out, "  Settings changes: %llu\n", 
            (unsigned long long)ctx->telemetry.settingsChangeCount);
    
    fprintf(out, "\nSecurity:\n");
    fprintf(out, "  Canaries enabled: %s\n", 
            ctx->settings.useCanaries ? "Yes" : "No");
    fprintf(out, "  Quarantine enabled: %s\n", 
            ctx->settings.useQuarantine ? "Yes" : "No");
    if (ctx->settings.useQuarantine) {
        fprintf(out, "  Quarantined blocks: %llu\n", 
                (unsigned long long)ctx->telemetry.quarantinedBlocks);
        fprintf(out, "  Quarantined bytes: %llu\n", 
                (unsigned long long)ctx->telemetry.quarantinedBytes);
    }
    fprintf(out, "  Corruption events: %llu\n", 
            (unsigned long long)ctx->telemetry.corruptionEvents);
    fprintf(out, "  Validation failures: %llu\n", 
            (unsigned long long)ctx->telemetry.validationFailures);
    
    fprintf(out, "\nLast Error: %s\n", ctx->telemetry.lastErrorMsg);
    fprintf(out, "=====================================\n");
}

bool canModifySetting(AllocatorContext* ctx, const char* setting_name) {
    if (!ctx->settings.allowRuntimeTuning) return false;
    
    /* Some settings are immutable after initialization */
    if (strcmp(setting_name, "bin_count") == 0 || 
        strcmp(setting_name, "alignment") == 0) {
        return false;
    }
    
    return true;
}

/* === DEFAULT SETTINGS GENERATOR === */

MemAllocSettings get_default_settings(OptimizationMode mode) {
    MemAllocSettings s = {0};
    
    /* Common defaults */
    s.mode = mode;
    s.bin_count = 32;
    s.alignment = 16;
    s.maxMetadataPages = 1000;
    s.ensureContiguous = false;
    s.allowRuntimeTuning = true;
    s.enableTelemetry = true;
    s.maxHeapSize = 0;  /* Unlimited */
    s.verboseErrors = true;
    s.errorCallback = NULL;
    s.callbackUserdata = NULL;
    
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
            s.quarantineSize = 0;
            s.useDeferredCoalescing = true;
            s.enableThreadCache = false;
            s.largeMmapThreshold = 128 * 1024;
            s.trackCallSites = false;
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
            s.quarantineSize = 0;
            s.useDeferredCoalescing = false;
            s.enableThreadCache = false;
            s.largeMmapThreshold = 256 * 1024;
            s.trackCallSites = false;
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
            s.quarantineSize = 0;
            s.useDeferredCoalescing = false;
            s.enableThreadCache = false;
            s.largeMmapThreshold = 192 * 1024;
            s.trackCallSites = true;
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
            s.enableThreadCache = false;
            s.largeMmapThreshold = 256 * 1024;
            s.trackCallSites = true;
            break;
    }
    
    return s;
}

/* === COMPLETE ALLOCATION IMPLEMENTATION === */

void* allocate_block(AllocatorContext** ctx_ref, size_t size, 
                     const char* file, int line) {
    AllocatorContext* ctx = *ctx_ref;
    
    /* Validate size */
    if (size == 0) {
        set_error(ctx, ERR_INVALID_SIZE, "Allocation size is 0 at %s:%d", 
                  file ? file : "unknown", line);
        return NULL;
    }
    
    /* Large allocation path - bypass normal allocator */
    if (ctx->settings.largeMmapThreshold > 0 && 
        size >= ctx->settings.largeMmapThreshold) {
        size_t pg_size = getPageSize();
        size_t alloc_size = ALIGN_UP(size, pg_size);
        
        void* ptr = allocPages(alloc_size);
        if (!ptr) {
            set_error(ctx, ERR_LARGE_ALLOC_FAILED,
                      "Failed to allocate %llu bytes via direct mapping at %s:%d",
                      (unsigned long long)size, file ? file : "unknown", line);
            return NULL;
        }
        
        track_large_alloc(ctx, ptr, alloc_size, size, file, line);
        
        if (ctx->settings.enableTelemetry) {
            ctx->telemetry.activeAllocations++;
            ctx->telemetry.allocationCount++;
            ctx->telemetry.totalAllocated += size;
        }
        
        return ptr;
    }
    
    /* Calculate overhead for normal allocation */
    size_t overhead = sizeof(BlockHeader);
    if (ctx->settings.useCanaries) {
        overhead += sizeof(uint32_t);  /* Bottom canary */
    }
    
    /* Check for overflow */
    if (size > SIZE_MAX - overhead) {
        set_error(ctx, ERR_SIZE_OVERFLOW,
                  "Requested size %llu + overhead %llu exceeds maximum at %s:%d",
                  (unsigned long long)size, (unsigned long long)overhead,
                  file ? file : "unknown", line);
        return NULL;
    }
    
    size_t actual = ALIGN_UP(size + overhead, ctx->settings.alignment);
    if (actual < MIN_BLOCK_SIZE) {
        actual = MIN_BLOCK_SIZE;
    }
    
    FreeBlock* chosen = NULL;
    
retry:
    /* Find suitable free block based on strategy */
    switch (ctx->settings.strategy) {
        case STRATEGY_BEST_FIT:
            chosen = find_best_fit(ctx, actual);
            break;
        case STRATEGY_FIRST_FIT:
            chosen = find_first_fit(ctx, actual);
            break;
        default:
            chosen = find_segregated_fit(ctx, actual);
            break;
    }
    
    /* No suitable block found - try to grow heap */
    if (!chosen) {
        if (grow_heap(ctx_ref, actual)) {
            ctx = *ctx_ref;
            goto retry;
        }
        return NULL;
    }
    
    /* Remove chosen block from free list */
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
        
        /* Add remainder to free list */
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
    
    /* Set bottom canary if enabled */
    if (ctx->settings.useCanaries) {
        uint32_t* bot_canary = (uint32_t*)((uint8_t*)chosen + 
                                           chosen->header.size - sizeof(uint32_t));
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
    
    /* Return pointer to user data (after header) */
    return (void*)((uint8_t*)chosen + sizeof(BlockHeader));
}

/* === COMPLETE DEALLOCATION IMPLEMENTATION === */

void free_block(AllocatorContext* ctx, void* ptr) {
    if (!ptr) return;
    
    /* Check if it's a large allocation */
    if (find_large_alloc(ctx, ptr)) {
        LargeAlloc* la = find_large_alloc(ctx, ptr);
        size_t requested = la->requested_size;
        
        if (untrack_large_alloc(ctx, ptr)) {
            if (ctx->settings.enableTelemetry) {
                ctx->telemetry.activeAllocations--;
                ctx->telemetry.freeCount++;
                ctx->telemetry.totalAllocated -= requested;
            }
        }
        return;
    }
    
    /* Normal allocation - get block header */
    BlockHeader* h = (BlockHeader*)((uint8_t*)ptr - sizeof(BlockHeader));
    
    /* Validate block */
    if (!validate_block(ctx, h)) {
        return;  /* Error already set by validate_block */
    }
    
    if (h->magic != MAGIC_ALLOCATED) {
        set_error(ctx, ERR_INVALID_POINTER,
                  "Pointer %p has invalid magic (not from this allocator)", ptr);
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
    
    /* Security features - zero or poison freed memory */
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
        size_t overhead = h->size - sizeof(BlockHeader) - payload_size;
        ctx->telemetry.totalOverhead -= overhead;
    }
    
    /* Mark as free and add to quarantine (which handles coalescing) */
    h->is_free = 1;
    h->magic = MAGIC_FREE;
    quarantine_push(ctx, h);
}

/* === COMPLETE REALLOC IMPLEMENTATION === */

void* realloc_block(AllocatorContext** ctx_ref, void* ptr, size_t new_size,
                    const char* file, int line) {
    AllocatorContext* ctx = *ctx_ref;
    
    /* Standard realloc semantics */
    if (!ptr) {
        return allocate_block(ctx_ref, new_size, file, line);
    }
    
    if (new_size == 0) {
        free_block(ctx, ptr);
        return NULL;
    }
    
    /* Check if it's a large allocation */
    if (find_large_alloc(ctx, ptr)) {
        /* For large allocations, just allocate new and copy */
        LargeAlloc* la = find_large_alloc(ctx, ptr);
        size_t old_size = la->requested_size;
        
        void* new_ptr = allocate_block(ctx_ref, new_size, file, line);
        if (new_ptr) {
            memcpy(new_ptr, ptr, MIN(old_size, new_size));
            free_block(*ctx_ref, ptr);
            if ((*ctx_ref)->settings.enableTelemetry) {
                (*ctx_ref)->telemetry.reallocCount++;
            }
        }
        return new_ptr;
    }
    
    /* Normal allocation */
    BlockHeader* h = (BlockHeader*)((uint8_t*)ptr - sizeof(BlockHeader));
    
    if (!validate_block(ctx, h)) {
        return NULL;
    }
    
    size_t old_size = h->requested_size;
    size_t overhead = sizeof(BlockHeader);
    if (ctx->settings.useCanaries) {
        overhead += sizeof(uint32_t);
    }
    
    size_t needed = ALIGN_UP(new_size + overhead, ctx->settings.alignment);
    if (needed < MIN_BLOCK_SIZE) {
        needed = MIN_BLOCK_SIZE;
    }
    
    /* If new size fits in current block, just update */
    if (needed <= h->size) {
        h->requested_size = new_size;
        if (ctx->settings.enableTelemetry) {
            ctx->telemetry.reallocCount++;
            ctx->telemetry.totalAllocated += (new_size - old_size);
        }
        return ptr;
    }
    
    /* Try expanding into next block if it's free */
    if (h->rel_next_phys != -1) {
        BlockHeader* next = (BlockHeader*)((uint8_t*)h + h->rel_next_phys);
        
        if (next->is_free && (h->size + next->size >= needed)) {
            bin_remove(ctx, (FreeBlock*)next);
            
            size_t combined = h->size + next->size;
            h->size = combined;
            h->rel_next_phys = next->rel_next_phys;
            
            if (next->rel_next_phys != -1) {
                BlockHeader* next_next = (BlockHeader*)((uint8_t*)next + 
                                                        next->rel_next_phys);
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
                ctx->telemetry.totalAllocated += (new_size - old_size);
                ctx->telemetry.coalescingEvents++;
            }
            
            return ptr;
        }
    }
    
    /* Need to allocate new block and copy */
    void* new_ptr = allocate_block(ctx_ref, new_size, file, line);
    if (!new_ptr) {
        set_error(*ctx_ref, ERR_RELOCATION_FAILED,
                  "Failed to allocate %llu bytes for realloc at %s:%d",
                  (unsigned long long)new_size, file ? file : "unknown", line);
        return NULL;
    }
    
    memcpy(new_ptr, ptr, MIN(old_size, new_size));
    free_block(*ctx_ref, ptr);
    
    if ((*ctx_ref)->settings.enableTelemetry) {
        (*ctx_ref)->telemetry.reallocCount++;
    }
    
    return new_ptr;
}


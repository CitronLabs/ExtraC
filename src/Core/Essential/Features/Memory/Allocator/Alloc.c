#include "Helpers.c"

errvt moduleMethod()
AllocError applyRuntimeSettings(AllocatorContext* ctx, MemAllocSettings* new_settings) {
    if (!ctx->settings.allowRuntimeTuning) {
        set_error(ctx, ERR_RUNTIME_TUNING_DISABLED, "Tuning disabled");
        return ERR_RUNTIME_TUNING_DISABLED;
    }
    
    /* Validate */
    if (!IS_POWER_OF_TWO(new_settings->alignment) || new_settings->alignment < 8) {
        set_error(ctx, ERR_INVALID_SETTINGS, "Invalid alignment");
        return ERR_INVALID_SETTINGS;
    }
    
    /* Immutable settings check */
    if (new_settings->bin_count != ctx->settings.bin_count ||
        new_settings->alignment != ctx->settings.alignment) {
        set_error(ctx, ERR_INCOMPATIBLE_SETTINGS, "Cannot change bin_count/alignment");
        return ERR_INCOMPATIBLE_SETTINGS;
    }
    
    /* Process pending operations */
    if (ctx->settings.useDeferredCoalescing) process_deferred_coalescing(ctx);
    
    /* Handle quarantine changes */
    if (ctx->settings.useQuarantine && !new_settings->useQuarantine) {
        quarantine_flush(ctx);
    } else if (!ctx->settings.useQuarantine && new_settings->useQuarantine) {
        ctx->quarantine = (Quarantine){NULL, NULL, 0, new_settings->quarantineSize, 0};
    } else if (new_settings->quarantineSize < ctx->quarantine.currentBytes) {
        while (ctx->quarantine.currentBytes > new_settings->quarantineSize) {
            quarantine_release_oldest(ctx);
        }
    }
    
    /* Apply */
    MemAllocSettings old = ctx->settings;
    ctx->settings = *new_settings;
    ctx->settings.bin_count = old.bin_count;
    ctx->settings.alignment = old.alignment;
    ctx->settings.allowRuntimeTuning = old.allowRuntimeTuning;
    
    ctx->telemetry.settingsChangeCount++;
    return ERR_NONE;
}

AllocError switchOptimizationMode(AllocatorContext* ctx, OptimizationMode mode) {
    if (!ctx->settings.allowRuntimeTuning) {
        set_error(ctx, ERR_RUNTIME_TUNING_DISABLED, "Tuning disabled");
        return ERR_RUNTIME_TUNING_DISABLED;
    }
    
    MemAllocSettings new_settings = get_default_settings(mode);
    new_settings.bin_count = ctx->settings.bin_count;
    new_settings.alignment = ctx->settings.alignment;
    new_settings.maxHeapSize = ctx->settings.maxHeapSize;
    new_settings.maxMetadataPages = ctx->settings.maxMetadataPages;
    new_settings.allowRuntimeTuning = true;
    new_settings.enableTelemetry = ctx->settings.enableTelemetry;
    new_settings.errorCallback = ctx->settings.errorCallback;
    new_settings.callbackUserdata = ctx->settings.callbackUserdata;
    
    return applyRuntimeSettings(ctx, &new_settings);
}

/* Individual setting modifiers (ALL IMPLEMENTED) */
#define SETTING_MODIFIER(name, type, field) \
    AllocError set##name(AllocatorContext* ctx, type value) { \
        if (!ctx->settings.allowRuntimeTuning) return ERR_RUNTIME_TUNING_DISABLED; \
        ctx->settings.field = value; \
        ctx->telemetry.settingsChangeCount++; \
        return ERR_NONE; \
    }

AllocError setStrategy(AllocatorContext *ctx, AllocStrategy value) {
  if (!ctx->settings.allowRuntimeTuning)
    return ERR_RUNTIME_TUNING_DISABLED;
  ctx->settings.strategy = value;
  ctx->telemetry.settingsChangeCount++;
  return ERR_NONE;
}

AllocError setMinSplitThreshold(AllocatorContext *ctx, size_t value) {
  if (!ctx->settings.allowRuntimeTuning)
    return ERR_RUNTIME_TUNING_DISABLED;
  ctx->settings.min_split_threshold = value;
  ctx->telemetry.settingsChangeCount++;
  return ERR_NONE;
}

AllocError setLargeMmapThreshold(AllocatorContext *ctx, size_t value) {
  if (!ctx->settings.allowRuntimeTuning)
    return ERR_RUNTIME_TUNING_DISABLED;
  ctx->settings.largeMmapThreshold = value;
  ctx->telemetry.settingsChangeCount++;
  return ERR_NONE;
}

AllocError setVerboseErrors(AllocatorContext *ctx, bool value) {
  if (!ctx->settings.allowRuntimeTuning)
    return ERR_RUNTIME_TUNING_DISABLED;
  ctx->settings.verboseErrors = value;
  ctx->telemetry.settingsChangeCount++;
  return ERR_NONE;
}

AllocError enableZeroOnFree(AllocatorContext* ctx, bool enable) {
    if (!ctx->settings.allowRuntimeTuning) return ERR_RUNTIME_TUNING_DISABLED;
    ctx->settings.zeroOnFree = enable;
    if (enable) ctx->settings.poisonOnFree = false;
    ctx->telemetry.settingsChangeCount++;
    return ERR_NONE;
}

AllocError enablePoisonOnFree(AllocatorContext* ctx, bool enable) {
    if (!ctx->settings.allowRuntimeTuning) return ERR_RUNTIME_TUNING_DISABLED;
    ctx->settings.poisonOnFree = enable;
    if (enable) ctx->settings.zeroOnFree = false;
    ctx->telemetry.settingsChangeCount++;
    return ERR_NONE;
}

AllocError enableCanaries(AllocatorContext* ctx, bool enable) {
    if (!ctx->settings.allowRuntimeTuning) return ERR_RUNTIME_TUNING_DISABLED;
    ctx->settings.useCanaries = enable;
    ctx->telemetry.settingsChangeCount++;
    return ERR_NONE;
}

AllocError enableValidation(AllocatorContext* ctx, bool enable) {
    if (!ctx->settings.allowRuntimeTuning) return ERR_RUNTIME_TUNING_DISABLED;
    ctx->settings.validateOnEntry = enable;
    ctx->telemetry.settingsChangeCount++;
    return ERR_NONE;
}

AllocError enableQuarantine(AllocatorContext* ctx, bool enable, size_t size) {
    if (!ctx->settings.allowRuntimeTuning) return ERR_RUNTIME_TUNING_DISABLED;
    if (!ctx->settings.useQuarantine && enable) {
        ctx->quarantine = (Quarantine){NULL, NULL, 0, size, 0};
    } else if (ctx->settings.useQuarantine && !enable) {
        quarantine_flush(ctx);
    }
    ctx->settings.useQuarantine = enable;
    ctx->settings.quarantineSize = size;
    ctx->telemetry.settingsChangeCount++;
    return ERR_NONE;
}

AllocError enableDeferredCoalescing(AllocatorContext* ctx, bool enable) {
    if (!ctx->settings.allowRuntimeTuning) return ERR_RUNTIME_TUNING_DISABLED;
    if (ctx->settings.useDeferredCoalescing && !enable) {
        process_deferred_coalescing(ctx);
    }
    ctx->settings.useDeferredCoalescing = enable;
    ctx->telemetry.settingsChangeCount++;
    return ERR_NONE;
}

AllocError setMaxHeapSize(AllocatorContext* ctx, len_t max) {
    if (!ctx->settings.allowRuntimeTuning) return ERR_RUNTIME_TUNING_DISABLED;
    if (max > 0 && max < ctx->total_size) {
        set_error(ctx, ERR_INVALID_SETTINGS, "Max smaller than current");
        return ERR_INVALID_SETTINGS;
    }
    ctx->settings.maxHeapSize = max;
    ctx->telemetry.settingsChangeCount++;
    return ERR_NONE;
}

/* PART 3: INITIALIZATION & UTILITIES (COMPLETE) */

AllocatorContext* createAllocator(OptimizationMode mode, size_t initial_size) {
    MemAllocSettings settings = get_default_settings(mode);
    size_t pg_size = getPageSize();
    size_t alloc_size = ALIGN_UP(initial_size, pg_size);
    void* mem = allocPages(alloc_size);
    if (!mem) return NULL;
    
    AllocatorContext* ctx = initAllocator(mem, alloc_size, settings);
    if (ctx) ctx->managed = false;
    return ctx;
}

AllocatorContext* initAllocator(void* base, size_t size, MemAllocSettings settings) {
    if (((uintptr_t)base & (settings.alignment - 1)) != 0) return NULL;
    if (!IS_POWER_OF_TWO(settings.alignment) || settings.alignment < 8) return NULL;
    if (settings.bin_count == 0 || settings.bin_count > 64) return NULL;
    if (settings.min_split_threshold < MIN_BLOCK_SIZE) {
        settings.min_split_threshold = MIN_BLOCK_SIZE;
    }
    
    size_t ctx_sz = ALIGN_UP(sizeof(AllocatorContext) + 
                             sizeof(intptr_t) * settings.bin_count,
                             settings.alignment);
    if (ctx_sz >= size) return NULL;
    
    AllocatorContext* ctx = (AllocatorContext*)base;
    memset(ctx, 0, ctx_sz);
    ctx->base_addr = base;
    ctx->total_size = size;
    ctx->settings = settings;
    ctx->managed = true;
    ctx->canary_seed = 0xDEADBEEF;
    ctx->deferred_threshold = 100;
    
    for (uint32_t i = 0; i < settings.bin_count; i++) {
        ctx->bin_offsets[i] = -1;
    }
    
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
    ctx->telemetry.totalMetadataBytes = ctx_sz;
    return ctx;
}

void destroyAllocator(AllocatorContext* ctx) {
    if (!ctx) return;
    if (!ctx->managed) {
        freePages(ctx->base_addr, ctx->total_size);
    }
}

/* PART 4: DIAGNOSTICS (COMPLETE) */

MemAllocSettings getCurrentSettings(AllocatorContext* ctx) {
    return ctx->settings;
}

len_t moduleMethod(AllocCtx, printAllocs, std_Stream* out) {
    fprintf(out, "=== Active Allocations ===\n");
    
    size_t ctx_sz = ALIGN_UP(sizeof(AllocatorContext) + 
                             sizeof(intptr_t) * this.settings.bin_count,
                             this.settings.alignment);
    BlockHeader* curr = (BlockHeader*)((uint8_t*)this.base_addr + ctx_sz);
    
    uint64_t count = 0;
    while (curr) {
        if (!curr->is_free) {
            count++;
            fprintf(out, "[%llu] %p: %llu bytes", 
                   count, 
                   (void*)((uint8_t*)curr + sizeof(BlockHeader)),
                   (unsigned long long)curr->requested_size);
            
            if (this.settings.trackCallSites && curr->alloc_file) {
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
    LargeAlloc* la = this.large_allocs;
    while (la) {
        count++;
        fprintf(out, "[%llu] %p: %llu bytes (large allocation)", 
               count, la->ptr, (unsigned long long)la->requested_size);
        if (this.settings.trackCallSites && la->alloc_file) {
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

len_t moduleMethod(AllocCtx, printDetailedStats, std_Stream* out) {
    fprintf(out, "=== Detailed Allocator Statistics ===\n");
    fprintf(out, "Current Mode: %s\n", 
            this.settings.mode == OPTIMIZE_SPEED ? "SPEED" :
            this.settings.mode == OPTIMIZE_SPACE ? "SPACE" :
            this.settings.mode == OPTIMIZE_BALANCED ? "BALANCED" : "SECURITY");
    fprintf(out, "Strategy: %s\n",
            this.settings.strategy == STRATEGY_SEGREGATED_FIT ? "Segregated-fit" :
            this.settings.strategy == STRATEGY_BEST_FIT ? "Best-fit" : "First-fit");
    
    fprintf(out, "\nMemory Usage:\n");
    fprintf(out, "  Total allocated: %llu bytes\n", 
            (unsigned long long)this.telemetry.totalAllocated);
    fprintf(out, "  Overhead: %llu bytes\n", 
            (unsigned long long)this.telemetry.totalOverhead);
    fprintf(out, "  Heap size: %llu bytes\n", 
            (unsigned long long)this.total_size);
    fprintf(out, "  Peak usage: %llu bytes\n", 
            (unsigned long long)this.telemetry.peakUsage);
    fprintf(out, "  Utilization: %.2f%%\n", 
            this.total_size > 0 ? 
            (100.0 * this.telemetry.totalAllocated / this.total_size) : 0.0);
    
    fprintf(out, "\nOperations:\n");
    fprintf(out, "  Allocations: %llu\n", 
            (unsigned long long)this.telemetry.allocationCount);
    fprintf(out, "  Frees: %llu\n", 
            (unsigned long long)this.telemetry.freeCount);
    fprintf(out, "  Reallocs: %llu\n", 
            (unsigned long long)this.telemetry.reallocCount);
    fprintf(out, "  Active: %llu\n", 
            (unsigned long long)this.telemetry.activeAllocations);
    fprintf(out, "  Peak active: %llu\n", 
            (unsigned long long)this.telemetry.peakActiveAllocations);
    
    fprintf(out, "\nOptimizations:\n");
    fprintf(out, "  Coalescing events: %llu\n", 
            (unsigned long long)this.telemetry.coalescingEvents);
    fprintf(out, "  Splitting events: %llu\n", 
            (unsigned long long)this.telemetry.splittingEvents);
    fprintf(out, "  Heap growths: %llu\n", 
            (unsigned long long)this.telemetry.heapGrowthEvents);
    fprintf(out, "  Deferred coalescing runs: %llu\n", 
            (unsigned long long)this.telemetry.deferredCoalescingRuns);
    fprintf(out, "  Settings changes: %llu\n", 
            (unsigned long long)this.telemetry.settingsChangeCount);
    
    fprintf(out, "\nSecurity:\n");
    fprintf(out, "  Canaries enabled: %s\n", 
            this.settings.useCanaries ? "Yes" : "No");
    fprintf(out, "  Quarantine enabled: %s\n", 
            this.settings.useQuarantine ? "Yes" : "No");
    if (this.settings.useQuarantine) {
        fprintf(out, "  Quarantined blocks: %llu\n", 
                (unsigned long long)this.telemetry.quarantinedBlocks);
        fprintf(out, "  Quarantined bytes: %llu\n", 
                (unsigned long long)this.telemetry.quarantinedBytes);
    }
    fprintf(out, "  Corruption events: %llu\n", 
            (unsigned long long)this.telemetry.corruptionEvents);
    fprintf(out, "  Validation failures: %llu\n", 
            (unsigned long long)this.telemetry.validationFailures);
    
    fprintf(out, "\nLast Error: %s\n", this.telemetry.lastErrorMsg);
    fprintf(out, "=====================================\n");
}

void dump_heap_stats(AllocatorContext* ctx, FILE* out) {
    fprintf(out, "=== Allocator Statistics ===\n");
    fprintf(out, "Mode: %s | Strategy: %s\n",
            ctx->settings.mode == OPTIMIZE_SPEED ? "SPEED" :
            ctx->settings.mode == OPTIMIZE_SPACE ? "SPACE" :
            ctx->settings.mode == OPTIMIZE_BALANCED ? "BALANCED" : "SECURITY",
            ctx->settings.strategy == STRATEGY_SEGREGATED_FIT ? "Segregated" :
            ctx->settings.strategy == STRATEGY_BEST_FIT ? "Best-fit" : "First-fit");
    fprintf(out, "Allocations: %llu | Frees: %llu | Active: %llu\n",
            (unsigned long long)ctx->telemetry.allocationCount,
            (unsigned long long)ctx->telemetry.freeCount,
            (unsigned long long)ctx->telemetry.activeAllocations);
    fprintf(out, "Total allocated: %llu bytes | Overhead: %llu bytes\n",
            (unsigned long long)ctx->telemetry.totalAllocated,
            (unsigned long long)ctx->telemetry.totalOverhead);
    fprintf(out, "Peak usage: %llu bytes | Heap size: %llu bytes\n",
            (unsigned long long)ctx->telemetry.peakUsage,
            (unsigned long long)ctx->total_size);
    fprintf(out, "Settings changes: %llu | Corruption events: %llu\n",
            (unsigned long long)ctx->telemetry.settingsChangeCount,
            (unsigned long long)ctx->telemetry.corruptionEvents);
    if (ctx->settings.useQuarantine) {
        fprintf(out, "Quarantined: %llu blocks, %llu bytes\n",
                (unsigned long long)ctx->telemetry.quarantinedBlocks,
                (unsigned long long)ctx->telemetry.quarantinedBytes);
    }
    fprintf(out, "===========================\n");
}

PRINT(std_Memory){
	
}

#undef module

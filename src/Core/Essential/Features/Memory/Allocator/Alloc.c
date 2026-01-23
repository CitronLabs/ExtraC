#include "Helpers.c"

mod(Settings) moduleFn(optimizeSettings)(mod(OptimizeMode) mode) {
    	switch (mode) {
        case Opt_Speed: 	{ return Setting_Speed;       }
        case Opt_Size:		{ return Setting_Size;        }
        case Opt_Secure:	{ return Setting_Secure;      }
        case Opt_SecureSpeed:	{ return Setting_SecureSpeed; }
        case Opt_SecureSize:	{ return Setting_SecureSize;  }
        case Opt_Balanced:	{ return Setting_Balanced;    }
    	}
}

errvt moduleMethod(AllocCtx, Edit_settings, mod(Settings) new){
    if (!this.settings.allowRuntimeTuning) 
        return mod(recordError)(self, ERR.MEM.RUNTIME_TUNING_DISABLED);
    
    /* Process pending operations */ 
    if (this.settings.useDeferredCoalescing) mod(processDeferredCoalescing)(self);
    
    /* Handle quarantine changes */
    if   (this.settings.useQuarantine && !new.useQuarantine)
        	mod(Quarantine_flush)(self);
    elif (!this.settings.useQuarantine && new.useQuarantine) 
        	this.quarantine = (Quarantine){nil, nil, 0, new.quarantineSize, 0};
    elif (new.quarantineSize < this.quarantine.currentBytes) 
        while (this.quarantine.currentBytes > new.quarantineSize) 
            	mod(Quarantine_releaseOldest)(self);
    
    
    /* Apply */
    mod(Settings) old 			= this.settings;
    this.settings 			= new;
    this.settings.binCount 		= old.binCount;
    this.settings.alignment 		= old.alignment;
    this.settings.allowRuntimeTuning 	= old.allowRuntimeTuning;
    
    this.telemetry.settingsChangeCount++;

return OK;
}


errvt moduleMethod(AllocCtx, Edit_setMinSplitThreshold, size_t value) {
  if (!this.settings.allowRuntimeTuning)
	return mod(recordError)(self, ERR.MEM.RUNTIME_TUNING_DISABLED);
  this.settings.minSplitThreshold = value;
  this.telemetry.settingsChangeCount++;
return OK;
}

errvt moduleMethod(AllocCtx, Edit_setLargeMmapThreshold, size_t value) {
  if (!this.settings.allowRuntimeTuning)
	return mod(recordError)(self, ERR.MEM.RUNTIME_TUNING_DISABLED);
  this.settings.largeMmapThreshold = value;
  this.telemetry.settingsChangeCount++;
return OK;
}

errvt moduleMethod(AllocCtx, Edit_setVerboseErrors, bool value) {
  if (!this.settings.allowRuntimeTuning)
	return mod(recordError)(self, ERR.MEM.RUNTIME_TUNING_DISABLED);
  this.settings.verboseErrors = value;
  this.telemetry.settingsChangeCount++;
return OK;
}

errvt moduleMethod(AllocCtx, Edit_enableZeroOnFree, bool enable) {
  if (!this.settings.allowRuntimeTuning)
	return mod(recordError)(self, ERR.MEM.RUNTIME_TUNING_DISABLED);

    this.settings.zeroOnFree = enable;
    if (enable) this.settings.poisonOnFree = false;
    this.telemetry.settingsChangeCount++;
  return OK;
}

errvt moduleMethod(AllocCtx, Edit_enablePoisonOnFree, bool enable) {
  if (!this.settings.allowRuntimeTuning)
	return mod(recordError)(self, ERR.MEM.RUNTIME_TUNING_DISABLED);

    this.settings.poisonOnFree = enable;
    if (enable) this.settings.zeroOnFree = false;
    this.telemetry.settingsChangeCount++;
  return OK;
}

errvt moduleMethod(AllocCtx, Edit_enableCanaries, bool enable) {
  if (!this.settings.allowRuntimeTuning)
	return mod(recordError)(self, ERR.MEM.RUNTIME_TUNING_DISABLED);

    this.settings.useCanaries = enable;
    this.telemetry.settingsChangeCount++;
  return OK;
}

errvt moduleMethod(AllocCtx, Edit_enableValidation, bool enable) {
  if (!this.settings.allowRuntimeTuning)
	return mod(recordError)(self, ERR.MEM.RUNTIME_TUNING_DISABLED);

    this.settings.validateOnEntry = enable;
    this.telemetry.settingsChangeCount++;
  return OK;
}

errvt moduleMethod(AllocCtx, Edit_enableQuarantine, bool enable, size_t size) {
  if (!this.settings.allowRuntimeTuning)
	return mod(recordError)(self, ERR.MEM.RUNTIME_TUNING_DISABLED);

    if (!this.settings.useQuarantine && enable) 
        this.quarantine = (Quarantine){nil, nil, 0, size, 0};
    elif (this.settings.useQuarantine && !enable)
        mod(Quarantine_flush)(self);

    this.settings.useQuarantine = enable;
    this.settings.quarantineSize = size;
    this.telemetry.settingsChangeCount++;
  return OK;
}

errvt moduleMethod(AllocCtx, Edit_enableDeferredCoalescing, bool enable) {
  if (!this.settings.allowRuntimeTuning)
	return mod(recordError)(self, ERR.MEM.RUNTIME_TUNING_DISABLED);

    if (this.settings.useDeferredCoalescing && !enable)
        mod(processDeferredCoalescing)(self);

    this.settings.useDeferredCoalescing = enable;
    this.telemetry.settingsChangeCount++;
  return OK;
}

errvt moduleMethod(AllocCtx, Edit_setMaxSize, len_t max) {
  if (!this.settings.allowRuntimeTuning)
	return mod(recordError)(self, ERR.MEM.RUNTIME_TUNING_DISABLED);

    if (max > 0 && max < this.total_size) {
        mod(recordError)(self, ERR.MEM.INVALID_SETTINGS);
	printlnErr("Max smaller than current");
        return ERR.MEM.INVALID_SETTINGS;
    }
    this.settings.maxSize = max;
    this.telemetry.settingsChangeCount++;
  return OK;
}
MemAllocSettings getCurrentSettings(AllocatorContext* ctx) {
    return this.settings;
}

/* PART 3: INITIALIZATION & UTILITIES (COMPLETE) */
/*
AllocatorContext* createAllocator(OptimizationMode mode, size_t initial_size) {
    MemAllocSettings settings = get_default_settings(mode);
    size_t pg_size = getPageSize();
    size_t alloc_size = ALIGN_UP(initial_size, pg_size);
    void* mem = allocPages(alloc_size);
    if (!mem) return nil;
    
    AllocatorContext* ctx = initAllocator(mem, alloc_size, settings);
    if (ctx) this.managed = false;
    return ctx;
}

AllocatorContext* initAllocator(void* base, size_t size, MemAllocSettings settings) {
    if (((uintptr_t)base & (settings.alignment - 1)) != 0) return nil;
    if (!IS_POWER_OF_TWO(settings.alignment) || settings.alignment < 8) return nil;
    if (settings.bin_count == 0 || settings.bin_count > 64) return nil;
    if (settings.min_split_threshold < MIN_BLOCK_SIZE) {
        settings.min_split_threshold = MIN_BLOCK_SIZE;
    }
    
    size_t ctx_sz = ALIGN_UP(sizeof(AllocatorContext) + 
                             sizeof(intptr_t) * settings.bin_count,
                             settings.alignment);
    if (ctx_sz >= size) return nil;
    
    AllocatorContext* ctx = (AllocatorContext*)base;
    memset(ctx, 0, ctx_sz);
    this.base_addr = base;
    this.total_size = size;
    this.settings = settings;
    this.managed = true;
    this.canary_seed = 0xDEADBEEF;
    this.deferred_threshold = 100;
    
    for (uint32_t i = 0; i < settings.bin_count; i++) {
        this.bin_offsets[i] = -1;
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
    initial->alloc_file = nil;
    initial->alloc_line = 0;
    initial->alloc_timestamp = 0;
    
    bin_insert(ctx, (FreeBlock*)initial);
    this.telemetry.totalMetadataBytes = ctx_sz;
    return ctx;
}

void destroyAllocator(AllocatorContext* ctx) {
    if (!ctx) return;
    if (!this.managed) {
        freePages(this.base_addr, this.total_size);
    }
}
*/
/* PART 4: DIAGNOSTICS (COMPLETE) */


len_t moduleMethod(AllocCtx, printAllocs, std_Stream* out) {
    len_t bytes_written = 0;

    bytes_written += printTo(out, "=== Active Allocations ===\n");
    
    size_t ctx_sz = ALIGN_UP(sizeof(AllocCtx) + 
                             sizeof(pntrval)  * this.settings.binCount,
                             this.settings.alignment);

    Block block = mod(parseBlock)(pntr_shiftcpy(self, ctx_sz));
    
    len_t count = 0;
    while (block.valid) {

        if (!block.header->is_free) {
            count++;
            bytes_written += printTo(out, 
		"[",$(count),"] ",$((pntr)block.free_data),": ",$(block.header->requested_size),"bytes");
            
            if (block.tracking) 
                bytes_written += printTo(out, 
		"\nTracked to: \n\t"
			"alloc_num = ",$(block.tracking->alloc_num), ",\n\t",
			"position = ", $use(std_ErrorPosition_Type, &block.tracking->position), ",\n\t",
			"time = ",     $use(XC_Time_Type, &block.tracking->time)
                );
            
            bytes_written += printTo(out, "\n");
        }
        
        if (block.header->rel_next_phys == -1) break;
        block = mod(parseBlock)(pntr_shiftcpy(block.header, block.header->rel_next_phys));
    }
    
    /* Also print large allocations */
    LargeAlloc* la = this.large_allocs;
    while (la) {
        count++;
        bytes_written += printTo(out, 
	    "[",$(count),"] ",$(la->ptr),": ",$(la->requested_size),"bytes");

        if (this.settings.trackCallSites) {
	    TrackingData* tracking =  pntr_shiftcpy(la, sizeof(LargeAlloc));
            bytes_written += printTo(out, 
		"\nTracked to: \n\t"
			"alloc_num = ",$(tracking->alloc_num), ",\n\t",
			"position = ", $use(std_ErrorPosition_Type, &tracking->position), ",\n\t",
			"time = ",     $use(XC_Time_Type, &tracking->time)
            );
        }
	bytes_written += printTo(out, "\n");
        la = la->next;
    }
    
    bytes_written += printTo(out, 
	    "Total active: ",$(count),"\n"
    	    "==========================\n"
   );

return bytes_written;
}

len_t moduleMethod(AllocCtx, printDetailedStats, std_Stream* out) {
    printTo(out, 
	"=== Detailed Allocator Statistics ===\n",
    	"Memory Usage:\n"
	"  Total allocated: ",	$(this.telemetry.totalAllocated)," bytes\n", 
    	"  Overhead: ",		$(this.telemetry.totalOverhead), " bytes\n", 
    	"  Heap size: ",	$(this.total_size),		 " bytes\n", 
    	"  Peak usage: ",	$(this.telemetry.peakUsage),	 " bytes\n", 
    	"  Utilization: ",	$F(this.telemetry.totalAllocated, .percentOf = this.total_size),"%\n\n"

    	"Operations:\n"
    	"  Allocations: ",	$(this.telemetry.allocationCount),	"\n", 
    	"  Frees: ",		$(this.telemetry.freeCount),		"\n", 
    	"  Reallocs: ",		$(this.telemetry.reallocCount),		"\n", 
    	"  Active: ",		$(this.telemetry.activeAllocations),	"\n", 
    	"  Peak active: ",	$(this.telemetry.peakActiveAllocations),"\n\n", 

    	"Optimizations:\n"
    	"  Coalescing events: ",$(this.telemetry.coalescingEvents),	 "\n", 
    	"  Splitting events: ", $(this.telemetry.splittingEvents), 	 "\n", 
    	"  Growths: ",		$(this.telemetry.growthEvents),		 "\n", 
    	"  Defer coalescings: ",$(this.telemetry.deferredCoalescingRuns),"\n", 
    	"  Settings changes: ", $(this.telemetry.settingsChangeCount),	 "\n\n", 

    	"Security:\n"
    	"  Canaries enabled: ",  this.settings.useCanaries ?   "Yes" : "No", "\n", 
    	"  Quarantine enabled: ",this.settings.useQuarantine ? "Yes" : "No", "\n"
    );
    if (this.settings.useQuarantine) {
        printTo(out, 
	"  Quarantined blocks: ", $(this.telemetry.quarantinedBlocks), 	"\n", 
        "  Quarantined bytes: ",  $(this.telemetry.quarantinedBytes),	"\n"
	);
    }
    printTo(out, 
    	"  Corruption events: ",  $(this.telemetry.corruptionEvents),	"\n", 
    	"  Validation failures: ",$(this.telemetry.validationFailures),	"\n", 
    	"=====================================\n"
    );
    
    
    
}

void dump_heap_stats(AllocatorContext* ctx, FILE* out) {
    fprintf(out, "=== Allocator Statistics ===\n");
    fprintf(out, "Mode: %s | Strategy: %s\n",
            this.settings.mode == OPTIMIZE_SPEED ? "SPEED" :
            this.settings.mode == OPTIMIZE_SPACE ? "SPACE" :
            this.settings.mode == OPTIMIZE_BALANCED ? "BALANCED" : "SECURITY",
            this.settings.strategy == STRATEGY_SEGREGATED_FIT ? "Segregated" :
            this.settings.strategy == STRATEGY_BEST_FIT ? "Best-fit" : "First-fit");
    fprintf(out, "Allocations: %llu | Frees: %llu | Active: %llu\n",
            (unsigned long long)this.telemetry.allocationCount,
            (unsigned long long)this.telemetry.freeCount,
            (unsigned long long)this.telemetry.activeAllocations);
    fprintf(out, "Total allocated: %llu bytes | Overhead: %llu bytes\n",
            (unsigned long long)this.telemetry.totalAllocated,
            (unsigned long long)this.telemetry.totalOverhead);
    fprintf(out, "Peak usage: %llu bytes | Heap size: %llu bytes\n",
            (unsigned long long)this.telemetry.peakUsage,
            (unsigned long long)this.total_size);
    fprintf(out, "Settings changes: %llu | Corruption events: %llu\n",
            (unsigned long long)this.telemetry.settingsChangeCount,
            (unsigned long long)this.telemetry.corruptionEvents);
    if (this.settings.useQuarantine) {
        fprintf(out, "Quarantined: %llu blocks, %llu bytes\n",
                (unsigned long long)this.telemetry.quarantinedBlocks,
                (unsigned long long)this.telemetry.quarantinedBytes);
    }
    fprintf(out, "===========================\n");
}

PRINT(std_Memory){
	
}

#undef module

#include "Helpers.c"

mod(Settings) moduleFn(optimizeSettings)(mod(OptimizeMode) mode) {
    	switch (mode) {
        case Opt_Speed: 	{ return Setting_Speed;       }
        case Opt_Size:		{ return Setting_Size;        }
        case Opt_Secure:	{ return Setting_Secure;      }
        case Opt_SecureSpeed:	{ return Setting_SecureSpeed; }
        case Opt_SecureSize:	{ return Setting_SecureSize;  }
        case Opt_Balanced:	{ return Setting_Balanced;    }
	default: {
	    ERR(ERR.INVALID, "Invalid optimization mode, defaulting to balanced...");
	    return Setting_Balanced; 
	}
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
        errvt err = mod(recordError)(self, ERR.MEM.INVALID_SETTINGS);
	printlnErr("Max smaller than current");
        return err;
    }
    this.settings.maxSize = max;
    this.telemetry.settingsChangeCount++;
  return OK;
}



/* PART 3: INITIALIZATION & UTILITIES (COMPLETE) */

errvt moduleMethod(AllocCtx, init, std_Memory* memory_object, mod(Settings) settings){
    if (!IS_POWER_OF_TWO(settings.alignment)){
	errvt err = mod(recordError)(self, ERR.MEM.INVALID_SETTINGS); 
	printlnErr("Alignment is not a power of two");
	return err;
    }

    if(settings.alignment < 8){
	errvt err =  mod(recordError)(self, ERR.MEM.INVALID_SETTINGS); 
	printlnErr("Alignment is less than 8");
	return err;
    }
    if (settings.binCount == 0){
	errvt err = mod(recordError)(self, ERR.MEM.INVALID_SETTINGS); 
	printlnErr("Bin count is 0");
	return err;
    }

    if(settings.binCount > 64){
	errvt err = mod(recordError)(self, ERR.MEM.INVALID_SETTINGS); 
	printlnErr("Bin count is greater than 64");
	return err;
    }

    if (settings.minSplitThreshold < MIN_BLOCK_SIZE)
        settings.minSplitThreshold = MIN_BLOCK_SIZE;
    
    len_t 
	ctx_size = ALIGN_UP(sizeof(AllocCtx) + 
                             (sizeof(pntrval) * settings.binCount),
                             settings.alignment),

	pg_size = core.System.Mem.getInfo().pageSize;
    

    if (ctx_size >= memory_object->pages * pg_size){
		// TODO: Figure out what to do here
    }
    
    memset(self, 0, ctx_size);
    this.memory_object 		= memory_object;
    this.total_size 		= memory_object->pages * pg_size;
    this.settings 		= settings;
    this.canary_seed 		= 0xDEADBEEF;
    this.deferred_threshold 	= 100;
    
    loop(i, settings.binCount)  this.bin_offsets[i] = nil;
    
    BlockHeader* init_block 	= pntr_shiftcpy(self, ctx_size);
    init_block->magic 		= MAGIC_FREE;
    init_block->canary_top 	= mod(getCanary)(self);
    init_block->size 		= this.total_size - ctx_size;
    init_block->requested_size 	= 0;
    init_block->is_free 	= 1;
    init_block->next 		= nil;
    init_block->prev 		= nil;
    
    mod(Bin_insert)(self, mod(parseBlock)(init_block));
    this.telemetry.totalMetadataBytes = ctx_size;

return OK;
}

errvt moduleMethod(std_Memory, setup, mod(Settings) settings){
    nonull(self) return err;


    iferr(mod(init)(this.pointer, self, settings)) return err;
    
    priv.managed = true;
return OK;
}

errvt moduleMethod(AllocCtx, destroy) {
	
    len_t 
	ctx_size = ALIGN_UP(sizeof(AllocCtx) + 
                               sizeof(pntrval)  * this.settings.binCount,
                               this.settings.alignment),

	pg_size = core.System.Mem.getInfo().pageSize;

    
    Block block = mod(parseBlock)(pntr_shiftcpy(self, ctx_size));
    void* last_contigous      = self;
    len_t last_contigous_size = 0;
    
    while (block.valid) {
	if(block.header->prev && !block.header->prev_contiguous){
	    core.System.Mem.dealloc(
			last_contigous, 
			ALIGN_UP(last_contigous_size, pg_size)
	    );
	    last_contigous 	= block.header;
	    last_contigous_size = 0;
	} 

	last_contigous_size += block.header->size;

	if(block.header->next != nil)
	    block = mod(parseBlock)(block.header->next);
	else
	    break;
    }
    
    core.System.Mem.dealloc(
	last_contigous, 
	ALIGN_UP(last_contigous_size, pg_size)
    );

return OK;
}

mod(Settings)  moduleMethod(std_Memory, getSettings){
	nonull(self) return (mod(Settings)){};

	if(!priv.managed){
	    ERR(ERR.INVALID, "Memory is not currently being managed by an allocator");
	    return (mod(Settings)){};
	}

	AllocCtx* ctx = this.pointer;
return ctx->settings;
}
mod(Telemetry) moduleMethod(std_Memory, getTelemetry){
	nonull(self) return (mod(Telemetry)){};

	if(!priv.managed){
	    ERR(ERR.INVALID, "Memory is not currently being managed by an allocator");
	    return (mod(Telemetry)){};
	}

	AllocCtx* ctx = this.pointer;
return ctx->telemetry;
}

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
			"position = ", $use(std_CodePos_Type, &block.tracking->position), ",\n\t",
			"time = ",     $use(XC_Time_Type, &block.tracking->time)
                );
            
            bytes_written += printTo(out, "\n");
        }
        
        if (block.header->next == nil) break;
        block = mod(parseBlock)(block.header->next);
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
			"position = ", $use(std_CodePos_Type, &tracking->position), ",\n\t",
			"time = ",     $use(XC_Time_Type, &tracking->time)
            );
        }
	bytes_written += printTo(out, "\n");
        la = la->next;
    }
    
    bytes_written += printTo(out, 
	    "Total active: ",$(count),"\n",
    	    "==========================\n"
   );

return bytes_written;
}

len_t moduleMethod(AllocCtx, printDetailedStats, std_Stream* out) {
    len_t bytes_written = 0;
    bytes_written += printTo(out, 
	"=== Detailed Allocator Statistics ===\n",
    	"Memory Usage:\n",
	"  Total allocated: ",	$(this.telemetry.totalAllocated)," bytes\n", 
    	"  Overhead: ",		$(this.telemetry.totalOverhead), " bytes\n", 
    	"  Heap size: ",	$(this.total_size),		 " bytes\n", 
    	"  Peak usage: ",	$(this.telemetry.peakUsage),	 " bytes\n", 
    	"  Utilization: ",	$F(this.telemetry.totalAllocated, .percentOf = this.total_size),"%\n\n",

    	"Operations:\n",
    	"  Allocations: ",	$(this.telemetry.allocationCount),	"\n", 
    	"  Frees: ",		$(this.telemetry.freeCount),		"\n", 
    	"  Reallocs: ",		$(this.telemetry.reallocCount),		"\n", 
    	"  Active: ",		$(this.telemetry.activeAllocations),	"\n", 
    	"  Peak active: ",	$(this.telemetry.peakActiveAllocations),"\n\n", 

    	"Optimizations:\n",
    	"  Coalescing events: ",$(this.telemetry.coalescingEvents),	 "\n", 
    	"  Splitting events: ", $(this.telemetry.splittingEvents), 	 "\n", 
    	"  Growths: ",		$(this.telemetry.growthEvents),		 "\n", 
    	"  Defer coalescings: ",$(this.telemetry.deferredCoalescingRuns),"\n", 
    	"  Settings changes: ", $(this.telemetry.settingsChangeCount),	 "\n\n", 

    	"Security:\n",
    	"  Canaries enabled: ",  this.settings.useCanaries ?   "Yes" : "No", "\n", 
    	"  Quarantine enabled: ",this.settings.useQuarantine ? "Yes" : "No", "\n"
    );
    if (this.settings.useQuarantine) {
        bytes_written += printTo(out, 
	"  Quarantined blocks: ", $(this.telemetry.quarantinedBlocks), 	"\n", 
        "  Quarantined bytes: ",  $(this.telemetry.quarantinedBytes),	"\n"
	);
    }
    bytes_written += printTo(out, 
    	"  Corruption events: ",  $(this.telemetry.corruptionEvents),	"\n", 
    	"  Validation failures: ",$(this.telemetry.validationFailures),	"\n", 
    	"=====================================\n"
    );

return bytes_written;
}

len_t moduleMethod(AllocCtx, printStats, std_Stream* out) {
    len_t bytes_written = 0;
    bytes_written += printTo(out, 
	"=== Allocator Statistics ===\n",
	"Allocations: ",$(this.telemetry.allocationCount)," | ",
	"Frees: ",      $(this.telemetry.freeCount),	  " | ",
	"Active: ",	$(this.telemetry.activeAllocations), "\n",

	"Total allocated: ", $(this.telemetry.totalAllocated)," bytes | ",
	"Overhead: ",	     $(this.telemetry.totalOverhead) ," bytes\n",

	"Peak usage: ",	     $(this.telemetry.peakUsage), " bytes | ",
	"Heap size: ",	     $(this.total_size),	  " bytes\n",

	"Settings changes: ",  $(this.telemetry.settingsChangeCount), " | ",
	"Corruption events: ", $(this.telemetry.corruptionEvents),    "\n"
    );
    if (this.settings.useQuarantine) {
        bytes_written += printTo(out, 
	    "Quarantined: ",
	    $(this.telemetry.quarantinedBlocks)," blocks, ",
	    $(this.telemetry.quarantinedBytes), " bytes\n",
	    "===========================\n"
	);
    }
return bytes_written;
}

PRINT(std_Memory){
	len_t bytes_written = 0;

	if(f.stats && priv.managed){
	    AllocCtx* allocator = this.pointer;
	    
	    if(f.detailed)
		bytes_written = mod(printDetailedStats)(allocator, out);
	    else
		bytes_written = mod(printStats)(allocator, out);
	} else {
	    bytes_written = printTo(out,
		"Memory address: ", $(this.pointer), ", ",
	     	"Memory length: ",  $(this.pages),   " pages"
	    );
	}

return bytes_written;
}


void* moduleIMethod(Interface_Alloc, len_t size, std_CodePos allocPos){
	self(std_Memory);
	if(!priv.managed) {
	    ERR(ERR.INVALID, "Memory is not currently being managed by an allocator");
	    return nil;
	}

return mod(allocateBlock)(this.pointer, size, allocPos);
}
errvt moduleIMethod(Interface_Free,  void* instance, std_CodePos freePos){
	self(std_Memory);
	if(!priv.managed) 
	    return ERR(ERR.INVALID, "Memory is not currently being managed by an allocator");
	

return mod(freeBlock)(this.pointer, instance, freePos);
}

void* moduleIMethod(Interface_Resize, void* instance, u64 size, std_CodePos allocPos){
	self(std_Memory);
	if(!priv.managed){
	    ERR(ERR.INVALID, "Memory is not currently being managed by an allocator");
	    return nil;
	}
	
return mod(reallocBlock)(this.pointer, instance, size, allocPos);
}

errvt moduleIMethod(Interface_setMax, len_t size){
	self(std_Memory);
	if(!priv.managed){
	    return ERR(ERR.INVALID, "Memory is not currently being managed by an allocator");
	}

return mod(Edit_setMaxSize)(this.pointer, size);
}
bool  moduleIMethod(Interface_isStatic){
	self(std_Memory);
	if(!priv.managed){
	    ERR(ERR.INVALID, "Memory is not currently being managed by an allocator");
	    return false;
	}

	AllocCtx* ctx = this.pointer;

return ctx->settings.maxSize ? ctx->total_size >= ctx->settings.maxSize : false;
}
len_t   moduleIMethod(Interface_getBytesAlloced){
	self(std_Memory);
	if(!priv.managed){
	    ERR(ERR.INVALID, "Memory is not currently being managed by an allocator");
	    return false;
	}

	AllocCtx* ctx = this.pointer;

return ctx->total_size;
}

#undef module

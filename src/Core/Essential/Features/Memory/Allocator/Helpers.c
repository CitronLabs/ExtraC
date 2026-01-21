#include "Alloc.h"

/* === ERROR HANDLING === */

static const char* DefaultErrMessage[] = {
    [ERR.NONE] 				= "No error",
    [ERR.MEM.OUT_OF_MEMORY] 		= "Out of memory: system cannot allocate more pages",
    [ERR.MEM.METADATA_CAP_EXCEEDED] 	= "Metadata capacity exceeded: hit maxMetadataPages limit",
    [ERR.MEM.HEAP_LIMIT_REACHED] 	= "Heap limit reached: allocation would exceed maxHeapSize",
    [ERR.MEM.INVALID_POINTER] 		= "Invalid pointer: pointer not from this allocator or corrupted",
    [ERR.MEM.DOUBLE_FREE] 		= "Double free detected: attempting to free already-freed memory",
    [ERR.MEM.CORRUPTION_DETECTED] 	= "Memory corruption detected: canary value mismatch or invalid magic",
    [ERR.MEM.ALIGNMENT_FAILURE] 	= "Alignment failure: requested alignment not power of 2 or too large",
    [ERR.MEM.RELOCATION_FAILED] 	= "Heap relocation failed: cannot ensure contiguous memory",
    [ERR.MEM.INVALID_SIZE] 		= "Invalid size: allocation size is 0 or negative",
    [ERR.MEM.OVERFLOW] 			= "Size overflow: requested size + overhead exceeds maximum",
    [ERR.MEM.UNALIGNED_BASE] 		= "Unaligned base: heap base address not properly aligned",
    [ERR.MEM.INVALID_SETTINGS] 		= "Invalid settings: configuration parameters are inconsistent",
    [ERR.MEM.RUNTIME_TUNING_DISABLED] 	= "Runtime tuning disabled: cannot change settings",
    [ERR.MEM.INCOMPATIBLE_SETTINGS] 	= "Incompatible settings: cannot apply requested configuration",
    [ERR.MEM.LARGE_ALLOC_FAILED] 	= "Large allocation failed: direct page allocation error"
};

void moduleMethod(AllocCtx, recordError, errvt err) {
    
	if (err == ERR.MEM.CORRUPTION_DETECTED) {
		this.telemetry.corruptionEvents++;
	}
    
    	ERR(err, generic DefaultErrMessage[err]);
}

/* === RANDOM NUMBER GENERATION === */

u32 xorshift32(u32* state) {
    u32 x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

u32 moduleMethod(AllocCtx, getCanary) {
    if (this.settings.useRandomCanaries) {
        return xorshift32(&this.canary_seed);
    }
    return CANARY_VALUE_BASE;
}

/* === BLOCK VALIDATION === */
alias(std.Memory.Allocator, Alloc);
Block moduleFn(parseBlock)(BlockHeader* start){

	Block result = {
	.valid 	   = true,
	.header    = start,
	.tracking  = start->tracked ? pntr_shiftcpy(start, sizeof(BlockHeader)) : nil
	};

	if(start->is_free){
		len_t offset = result.tracking == nil ? 
				sizeof(BlockHeader) : 
				sizeof(BlockHeader) + sizeof(TrackingData)
		;

		result.free_data = pntr_shiftcpy(start, offset);
	}

	var heap = new(std_Memory, MemPage(1));

	Alloc.setup(&heap, Alloc.optimizeSettings(Alloc.Optimize.SECURITY));

	strc8 hello = Alloc.Interface.Alloc(heap, sizeof("Hello, World"), getErrorPos());

	std.Memory.copyTo(hello, "Hello, World", sizeof("Hello, World"));

return result;
}

Block moduleMethod(AllocCtx, getBlock, pntrval rel_pntr){
	BlockHeader* start = REL_TO_PTR(self, rel_pntr);

return start ? mod(parseBlock)(start): (Block){0};
}

bool moduleMethod(AllocCtx, isValidBlock, BlockHeader* blk, bool checkCanaries) {

    /* Check magic number */
    if (blk->magic != MAGIC_ALLOCATED && blk->magic != MAGIC_FREE){
            mod(recordError)(self, ERR.MEM.CORRUPTION_DETECTED); printlnErr(
            "Invalid magic number at ", $(blk), ": ",
                "expected ",     $F(MAGIC_ALLOCATED, .base = 16), " "
                "or ",        $F(MAGIC_FREE,         .base = 16), ", "
                "but got ",    $F(blk->magic,         .base = 16)    
        );

            this.telemetry.validationFailures++;
            return false;
    }
    
    /* Check size sanity */
    if (blk->size < sizeof(BlockHeader) || blk->size > this.total_size) {
            mod(recordError)(self, ERR.MEM.CORRUPTION_DETECTED); printlnErr(
            "Invalid block size at ", $(blk), ": "
                "block size: ",   $(blk->size), ", "
                    "total heap: ",   $(this.total_size)
            );

            this.telemetry.validationFailures++;
            return false;
    }
    
    /* Check canaries if enabled */
    if (checkCanaries && this.settings.useCanaries && !blk->is_free) {
        u32* canary_bot = pntr_shiftcpy(blk, blk->size - sizeof(u32));

        if (*canary_bot != blk->canary_top) {
            mod(recordError)(self, ERR.MEM.CORRUPTION_DETECTED); printlnErr(
                      "Buffer Overflow Detected :: "
                "Bottom canary corrupted at ", $(blk), ": "
                "expected ",     $F(blk->canary_top, .base = 16), ", "
                "but got ",    $F(*canary_bot,     .base = 16)
            );

            this.telemetry.validationFailures++;
            return false;
        }
    }
    
return true;
}

/* === BIN MANAGEMENT === */

len_t moduleMethod(AllocCtx, Bin_getIndex, len_t size){
    if (size <= this.settings.alignment) return 0;
    
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
    if (index >= this.settings.binCount) {
        index = this.settings.binCount - 1;
    }
    
return index;
}

void moduleMethod(AllocCtx, Bin_insert, Block block) {

    len_t idx 		= mod(Bin_getIndex)(self, block.header->size);

    pntrval block_rel 	= PTR_TO_REL(self, block.header);
    FreeData* head 	= mod(getBlock)(self, this.bin_offsets[idx]).free_data;
    
    block.free_data->rel_next_free = this.bin_offsets[idx];
    block.free_data->rel_prev_free = -1;
    
    if (head) {
        head->rel_prev_free = block_rel;
    }
    
    this.bin_offsets[idx] = block_rel;
    block.header->magic   = MAGIC_FREE;
}

void moduleMethod(AllocCtx, Bin_remove, Block block) {

    len_t idx = mod(Bin_getIndex)(self, block.header->size);
    
    FreeData* prev = mod(getBlock)(self, block.free_data->rel_prev_free).free_data;
    FreeData* next = mod(getBlock)(self, block.free_data->rel_next_free).free_data;
    
    if (prev) {
        prev->rel_next_free   = block.free_data->rel_next_free;
    } else {
        this.bin_offsets[idx] = block.free_data->rel_next_free;
    }
    
    if (next) {
        next->rel_prev_free = block.free_data->rel_prev_free;
    }
    
    block.free_data->rel_next_free = -1;
    block.free_data->rel_prev_free = -1;
}

/* === ALLOCATION STRATEGIES === */

Block moduleMethod(AllocCtx, findFit, size_t size) {
    /* Search bins starting from appropriate size class */
    loopat(i, mod(Bin_getIndex)(self, size), this.settings.binCount) {
        Block curr = mod(getBlock)(self, this.bin_offsets[i]);
        
        while (curr.valid) {
            if (this.settings.validateOnEntry && !mod(isValidBlock)(self, curr.header, true)) {
                return (Block){0};
            }
            
            if (curr.header->size >= size) {
                return curr;
            }
            
            curr = mod(getBlock)(self, curr.free_data->rel_next_free);
        }
    }
    
return (Block){0};
}

/* === COALESCING === */

void moduleMethod(AllocCtx, coalesceBlock, Block blk) {
    /* Coalesce with next physical block if it's free */

    BlockHeader* header = blk.header;

    if (header->rel_next_phys != -1) {
        BlockHeader* next = pntr_shiftcpy(header, header->rel_next_phys);
        
        if (next->is_free && mod(isValidBlock)(self, next, false)) {
            /* Remove next from free list */
            mod(Bin_remove)(self, mod(parseBlock)(next));
            
            /* Merge sizes */
            header->size += next->size;
            header->rel_next_phys = next->rel_next_phys;
            
            /* Update the block after next to point back to header */
            if (next->rel_next_phys != -1) {
                BlockHeader* next_next = pntr_shiftcpy(next, next->rel_next_phys);
                next_next->rel_prev_phys = PTR_TO_REL(next_next, header);
            }
            
            this.telemetry.coalescingEvents++;
        }
    }
    
    /* Coalesce with previous physical block if it's free */
    if (header->rel_prev_phys != -1) {
        BlockHeader* prev     = pntr_shiftcpy(header, header->rel_prev_phys);
        Block 	     prev_blk = mod(parseBlock)(prev);

        if (prev->is_free && mod(isValidBlock)(self, prev, false)) {
            /* Remove prev from free list */
            mod(Bin_remove)(self, prev_blk);
            
            /* Merge sizes */
            prev->size += header->size;
            prev->size =  header->rel_next_phys;
            
            /* Update the block after header to point back to prev */
            if (header->rel_next_phys != -1) {
                BlockHeader* next = pntr_shiftcpy(header, header->rel_next_phys);
                next->rel_prev_phys = PTR_TO_REL(next, prev);
            }
            
            /* Continue with prev as the merged block */
            blk = prev_blk;
            this.telemetry.coalescingEvents++;
        }
    }
    /* Insert the (potentially coalesced) block back into free list */
    mod(Bin_insert)(self, blk);
}

/* === DEFERRED COALESCING === */

void moduleMethod(AllocCtx, processDeferredCoalescing){
    if (!this.deferred_list) return;
    
    DeferredBlock* curr = this.deferred_list;
    
    while (curr) {
        Block blk = mod(parseBlock)(curr->block);
        
        if (blk.header->is_free) {
            /* Remove from free list and coalesce */
            mod(Bin_remove)(self,    blk);
            mod(coalesceBlock)(self, blk);
        }
        
        curr = curr->next;
    }
    
    /* Clear deferred list */
    this.deferred_list  = nil;
    this.deferred_count = 0;
    this.telemetry.deferredCoalescingRuns++;
}

void moduleMethod(AllocCtx, addToDeferredList, Block blk) {
    /* Use space within the block header for deferred list node */
    DeferredBlock* node = pntr_shiftcpy(blk.header, sizeof(BlockHeader));
    node->block = blk.header;
    node->next  = this.deferred_list;
    this.deferred_list = node;
    this.deferred_count++;
    
    /* Still add to free list immediately */
    mod(Bin_insert)(self, blk);
    
    /* Process deferred list if threshold reached */
    if (this.deferred_count >= this.deferred_threshold) {
        mod(processDeferredCoalescing)(self);
    }
}

/* === QUARANTINE MANAGEMENT === */

void moduleMethod(AllocCtx, Quarantine_releaseOldest){
    if (!this.quarantine.head) return;
    
    QuarantineNode* node = this.quarantine.head;
    this.quarantine.head = node->next;
    
    if (!this.quarantine.head) {
        this.quarantine.tail = nil;
    }
    
    BlockHeader* blk = node->block_ptr;
    this.quarantine.currentBytes -= blk->size;
    this.quarantine.nodeCount--;
    this.telemetry.quarantinedBlocks--;
    
    /* Actually free the block now */
    blk->is_free = 1;
    blk->magic   = MAGIC_FREE;
    
    Block parsedBlock = mod(parseBlock)(blk);

    if (this.settings.useDeferredCoalescing)
        mod(addToDeferredList)(self, parsedBlock);
    else
        mod(coalesceBlock)(self, parsedBlock);
}

void moduleMethod(AllocCtx, Quarantine_flush){
    while (this.quarantine.head) {
        mod(Quarantine_releaseOldest)(self);
    }
}

void moduleMethod(AllocCtx, Quarantine_push, Block blk){
    if (!this.settings.useQuarantine) {
        /* Quarantine disabled - free immediately */
        blk.header->is_free = 1;
        blk.header->magic = MAGIC_FREE;
        
        if (this.settings.useDeferredCoalescing) 
            mod(addToDeferredList)(self, blk);
        else 
            mod(coalesceBlock)(self, blk);
        
        return;
    }
    
    /* Release oldest blocks if over capacity */
    int safety_counter = 0;
    while (this.quarantine.currentBytes + blk.header->size > this.quarantine.maxBytes &&
           this.quarantine.head != nil) {
	mod(Quarantine_releaseOldest)(self);
        
        /* Safety check to prevent infinite loops */
        if (++safety_counter > 10000) {
            /* Emergency: flush entire quarantine */
            mod(Quarantine_flush)(self);
            break;
        }
    }
    
    /* If block is too large for quarantine, just free it directly */
    if (blk.header->size > this.quarantine.maxBytes) {
        blk.header->is_free = 1;
        blk.header->magic = MAGIC_FREE;
        if (this.settings.useDeferredCoalescing) {
            mod(addToDeferredList)(self, blk);
        } else {
            mod(coalesceBlock)(self, blk);
        }
        return;
    }
    
    /* Create quarantine node within the block itself */
    QuarantineNode* node = pntr_shiftcpy(blk.header, sizeof(BlockHeader));
    node->block_ptr 	 = blk.header;
    node->size 		 = blk.header->size;
    node->timestamp 	 = this.alloc_counter;
    node->next 		 = nil;
    
    /* Add to tail of quarantine queue */
    if (this.quarantine.tail) {
        this.quarantine.tail->next = node;
    } else {
        this.quarantine.head = node;
    }
    this.quarantine.tail = node;
    
    /* Update quarantine stats */
    this.quarantine.currentBytes += blk.header->size;
    this.quarantine.nodeCount++;
    this.telemetry.quarantinedBlocks++;
    this.telemetry.quarantinedBytes = this.quarantine.currentBytes;
}


/* === LARGE ALLOCATION TRACKING === */

void moduleMethod(AllocCtx, LargeAlloc_track, 
	LargeAlloc* 	  la, 
	size_t 		  size, 
	size_t 		  requested, 
	std_ErrorPosition allocPos
) {
    /* Allocate tracking structure */
    la->ptr 		= pntr_shiftcpy(la, sizeof(LargeAlloc));
    la->size 		= size;
    la->requested_size = requested;

    if(this.settings.trackCallSites){
	TrackingData* tracking = la->ptr;
	pntr_shift(la->ptr, sizeof(TrackingData));

    	tracking->position  = allocPos;
    	tracking->alloc_num = ++this.alloc_counter;
	
	core.System.Time.getNow(
		core.System.Time.Source.REALTIME, 
		&tracking->time
	);
    }

    /* Add to head of list */
    la->next = this.large_allocs;

    this.large_allocs->prev = la;
    this.large_allocs       = la;
    
    this.telemetry.largeAllocations++;
}

errvt moduleMethod(AllocCtx, LargeAlloc_untrack, LargeAlloc* la) {
    LargeAlloc** curr = &this.large_allocs;
    
    /* Remove from list */
    *curr = (*curr)->next;
    
    /* Free tracking structure and memory */
    core.System.Mem.dealloc(la, la->size);
    
    this.telemetry.largeAllocations--;

return OK;
}

/* === HEAP GROWTH === */

errvt moduleMethod(AllocCtx, grow, size_t needed) {
    AllocatorContext* ctx = *ctx_ref;
    
    /* Cannot grow managed (fixed-size) heaps */
    if (this.managed) {
        set_error(ctx, ERR_HEAP_LIMIT_REACHED,
                  "Cannot grow managed heap (fixed size: %llu bytes)",
                  (unsigned long long)this.total_size);
        return false;
    }

    size_t pg_size = getPageSize();
    size_t req_size = ALIGN_UP(needed, pg_size);
    
    /* Check heap size limit */
    if (this.settings.maxHeapSize > 0 && 
        (this.total_size + req_size > this.settings.maxHeapSize)) {
        set_error(ctx, ERR_HEAP_LIMIT_REACHED,
                  "Allocation would exceed maxHeapSize (%llu + %llu > %llu)",
                  (unsigned long long)this.total_size,
                  (unsigned long long)req_size,
                  (unsigned long long)this.settings.maxHeapSize);
        return false;
    }

    /* Check page limit */
    if (this.telemetry.metadataPageCount + 1 > this.settings.maxPages) {
        set_error(ctx, ERR_METADATA_CAP_EXCEEDED,
                  "Would exceed maxMetadataPages limit (%llu)",
                  (unsigned long long)this.settings.maxMetadataPages);
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
    bool contiguous = (new_mem == (uint8_t*)this.base_addr + this.total_size);

    /* Handle contiguous requirement */
    if (this.settings.ensureContiguous && !contiguous) {
        /* Need to relocate entire heap to ensure contiguity */
        size_t new_total = this.total_size + req_size;
        void* big_strip = allocPages(new_total);
        
        if (!big_strip) {
            freePages(new_mem, req_size);
            set_error(ctx, ERR_RELOCATION_FAILED,
                      "Cannot allocate contiguous block of %llu bytes",
                      (unsigned long long)new_total);
            return false;
        }

        /* Copy existing heap to new location */
        memcpy(big_strip, this.base_addr, this.total_size);
        
        /* Free old heap and unused new memory */
        freePages(this.base_addr, this.total_size);
        freePages(new_mem, req_size);

        /* Update context */
        ctx = (AllocatorContext*)big_strip;
        this.base_addr = big_strip;
        this.total_size = new_total;
        *ctx_ref = ctx;
    } else {
        /* Just expand total size */
        this.total_size += req_size;
    }

    /* Create new free block at end of heap */
    BlockHeader* new_h = (BlockHeader*)((uint8_t*)this.base_addr + 
                                        (this.total_size - req_size));
    new_h->magic = MAGIC_FREE;
    new_h->canary_top = get_canary(ctx);
    new_h->size = req_size;
    new_h->requested_size = 0;
    new_h->is_free = 1;
    new_h->rel_next_phys = -1;
    new_h->rel_prev_phys = -1;
    new_h->alloc_file = nil;
    new_h->alloc_line = 0;
    new_h->alloc_timestamp = 0;
    
    /* If contiguous, link to existing heap and try to coalesce */
    if (contiguous) {
        size_t ctx_sz = ALIGN_UP(sizeof(AllocatorContext) + 
                                 sizeof(intptr_t) * this.settings.bin_count,
                                 this.settings.alignment);
        BlockHeader* curr = (BlockHeader*)((uint8_t*)this.base_addr + ctx_sz);
        
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
    this.telemetry.metadataPageCount = (this.total_size + pg_size - 1) / pg_size;
    this.telemetry.heapGrowthEvents++;
    
    return true;
}

/* === DIAGNOSTICS === */


bool canModifySetting(AllocatorContext* ctx, const char* setting_name) {
    if (!this.settings.allowRuntimeTuning) return false;
    
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
    s.errorCallback = nil;
    s.callbackUserdata = nil;
    
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
        return nil;
    }
    
    /* Large allocation path - bypass normal allocator */
    if (this.settings.largeMmapThreshold > 0 && 
        size >= this.settings.largeMmapThreshold) {
        size_t pg_size = getPageSize();
        size_t alloc_size = ALIGN_UP(size, pg_size);
        
        void* ptr = allocPages(alloc_size);
        if (!ptr) {
            set_error(ctx, ERR_LARGE_ALLOC_FAILED,
                      "Failed to allocate %llu bytes via direct mapping at %s:%d",
                      (unsigned long long)size, file ? file : "unknown", line);
            return nil;
        }
        
        track_large_alloc(ctx, ptr, alloc_size, size, file, line);
        
        if (this.settings.enableTelemetry) {
            this.telemetry.activeAllocations++;
            this.telemetry.allocationCount++;
            this.telemetry.totalAllocated += size;
        }
        
        return ptr;
    }
    
    /* Calculate overhead for normal allocation */
    size_t overhead = sizeof(BlockHeader);
    if (this.settings.useCanaries) {
        overhead += sizeof(uint32_t);  /* Bottom canary */
    }
    
    /* Check for overflow */
    if (size > SIZE_MAX - overhead) {
        set_error(ctx, ERR_SIZE_OVERFLOW,
                  "Requested size %llu + overhead %llu exceeds maximum at %s:%d",
                  (unsigned long long)size, (unsigned long long)overhead,
                  file ? file : "unknown", line);
        return nil;
    }
    
    size_t actual = ALIGN_UP(size + overhead, this.settings.alignment);
    if (actual < MIN_BLOCK_SIZE) {
        actual = MIN_BLOCK_SIZE;
    }
    
    FreeBlock* chosen = nil;
    
retry:
    /* Find suitable free block based on strategy */
    switch (this.settings.strategy) {
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
        return nil;
    }
    
    /* Remove chosen block from free list */
    bin_remove(ctx, chosen);
    
    /* Split block if remainder is large enough */
    if (chosen->header.size - actual >= this.settings.min_split_threshold) {
        FreeBlock* remainder = (FreeBlock*)((uint8_t*)chosen + actual);
        
        remainder->header.magic = MAGIC_FREE;
        remainder->header.canary_top = get_canary(ctx);
        remainder->header.size = chosen->header.size - actual;
        remainder->header.requested_size = 0;
        remainder->header.is_free = 1;
        remainder->header.rel_next_phys = chosen->header.rel_next_phys;
        remainder->header.rel_prev_phys = PTR_TO_REL(remainder, chosen);
        remainder->header.alloc_file = nil;
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
        this.telemetry.splittingEvents++;
    }
    
    /* Finalize allocated block */
    chosen->header.magic = MAGIC_ALLOCATED;
    chosen->header.is_free = 0;
    chosen->header.requested_size = size;
    chosen->header.alloc_timestamp = ++this.alloc_counter;
    
    if (this.settings.trackCallSites) {
        chosen->header.alloc_file = file;
        chosen->header.alloc_line = line;
    }
    
    /* Set bottom canary if enabled */
    if (this.settings.useCanaries) {
        uint32_t* bot_canary = (uint32_t*)((uint8_t*)chosen + 
                                           chosen->header.size - sizeof(uint32_t));
        *bot_canary = chosen->header.canary_top;
    }
    
    /* Update telemetry */
    if (this.settings.enableTelemetry) {
        this.telemetry.activeAllocations++;
        this.telemetry.allocationCount++;
        this.telemetry.totalAllocated += size;
        this.telemetry.totalOverhead += (actual - size);
        
        if (this.telemetry.activeAllocations > this.telemetry.peakActiveAllocations) {
            this.telemetry.peakActiveAllocations = this.telemetry.activeAllocations;
        }
        
        len_t current_usage = this.telemetry.totalAllocated + this.telemetry.totalOverhead;
        if (current_usage > this.telemetry.peakUsage) {
            this.telemetry.peakUsage = current_usage;
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
            if (this.settings.enableTelemetry) {
                this.telemetry.activeAllocations--;
                this.telemetry.freeCount++;
                this.telemetry.totalAllocated -= requested;
            }
        }
        return;
    }
    
    /* Normal allocation - get block header */
    BlockHeader* blk = (BlockHeader*)((uint8_t*)ptr - sizeof(BlockHeader));
    
    /* Validate block */
    if (!validate_block(ctx, blk)) {
        return;  /* Error already set by validate_block */
    }
    
    if (blk->magic != MAGIC_ALLOCATED) {
        set_error(ctx, ERR_INVALID_POINTER,
                  "Pointer %p has invalid magic (not from this allocator)", ptr);
        return;
    }
    
    if (blk->is_free) {
        set_error(ctx, ERR_DOUBLE_FREE,
                  "Double free detected at %p (allocated at %s:%d, timestamp %llu)",
                  ptr, blk->alloc_file ? blk->alloc_file : "unknown",
                  blk->alloc_line, (unsigned long long)blk->alloc_timestamp);
        return;
    }
    
    size_t payload_size = blk->requested_size;
    
    /* Security features - zero or poison freed memory */
    if (this.settings.zeroOnFree && !this.settings.poisonOnFree) {
        memset(ptr, 0, payload_size);
    } else if (this.settings.poisonOnFree) {
        memset(ptr, 0xDE, payload_size);
    }
    
    /* Update telemetry */
    if (this.settings.enableTelemetry) {
        this.telemetry.activeAllocations--;
        this.telemetry.freeCount++;
        this.telemetry.totalAllocated -= payload_size;
        size_t overhead = blk->size - sizeof(BlockHeader) - payload_size;
        this.telemetry.totalOverhead -= overhead;
    }
    
    /* Mark as free and add to quarantine (which handles coalescing) */
    blk->is_free = 1;
    blk->magic = MAGIC_FREE;
    quarantine_push(ctx, blk);
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
        return nil;
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
    BlockHeader* blk = (BlockHeader*)((uint8_t*)ptr - sizeof(BlockHeader));
    
    if (!validate_block(ctx, blk)) {
        return nil;
    }
    
    size_t old_size = blk->requested_size;
    size_t overhead = sizeof(BlockHeader);
    if (this.settings.useCanaries) {
        overhead += sizeof(uint32_t);
    }
    
    size_t needed = ALIGN_UP(new_size + overhead, this.settings.alignment);
    if (needed < MIN_BLOCK_SIZE) {
        needed = MIN_BLOCK_SIZE;
    }
    
    /* If new size fits in current block, just update */
    if (needed <= blk->size) {
        blk->requested_size = new_size;
        if (this.settings.enableTelemetry) {
            this.telemetry.reallocCount++;
            this.telemetry.totalAllocated += (new_size - old_size);
        }
        return ptr;
    }
    
    /* Try expanding into next block if it's free */
    if (blk->rel_next_phys != -1) {
        BlockHeader* next = (BlockHeader*)((uint8_t*)blk + blk->rel_next_phys);
        
        if (next->is_free && (blk->size + next->size >= needed)) {
            bin_remove(ctx, (FreeBlock*)next);
            
            size_t combined = blk->size + next->size;
            blk->size = combined;
            blk->rel_next_phys = next->rel_next_phys;
            
            if (next->rel_next_phys != -1) {
                BlockHeader* next_next = (BlockHeader*)((uint8_t*)next + 
                                                        next->rel_next_phys);
                next_next->rel_prev_phys = PTR_TO_REL(next_next, blk);
            }
            
            blk->requested_size = new_size;
            
            /* Update bottom canary */
            if (this.settings.useCanaries) {
                uint32_t* bot = (uint32_t*)((uint8_t*)blk + blk->size - sizeof(uint32_t));
                *bot = blk->canary_top;
            }
            
            if (this.settings.enableTelemetry) {
                this.telemetry.reallocCount++;
                this.telemetry.totalAllocated += (new_size - old_size);
                this.telemetry.coalescingEvents++;
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
        return nil;
    }
    
    memcpy(new_ptr, ptr, MIN(old_size, new_size));
    free_block(*ctx_ref, ptr);
    
    if ((*ctx_ref)->settings.enableTelemetry) {
        (*ctx_ref)->telemetry.reallocCount++;
    }
    
    return new_ptr;
}

/* === WRAPPER FUNCTIONS === */

void* std_malloc_debug(AllocatorContext** ctx, size_t size, const char* file, int line) {
    return allocate_block(ctx, size, file, line);
}
void* std_malloc(AllocatorContext** ctx, size_t size) {
    return allocate_block(ctx, size, nil, 0);
}

void* std_calloc(AllocatorContext** ctx, size_t nmemb, size_t size) {
    /* Check for overflow */
    if (nmemb != 0 && size > SIZE_MAX / nmemb) {
        set_error(*ctx, ERR_SIZE_OVERFLOW,
                  "calloc overflow: %llu * %llu exceeds maximum",
                  (unsigned long long)nmemb, (unsigned long long)size);
        return nil;
    }
    
    size_t total = nmemb * size;
    void* ptr = allocate_block(ctx, total, nil, 0);
    
    if (ptr) {
        memset(ptr, 0, total);
    }
    
    return ptr;
}

void* std_realloc(AllocatorContext** ctx, void* ptr, size_t size) {
    return realloc_block(ctx, ptr, size, nil, 0);
}

void* std_aligned_alloc(AllocatorContext** ctx, size_t alignment, size_t size) {
    if (!IS_POWER_OF_TWO(alignment)) {
        set_error(*ctx, ERR_ALIGNMENT_FAILURE,
                  "Alignment %llu is not a power of 2",
                  (unsigned long long)alignment);
        return nil;
    }
    
    if (alignment > getPageSize()) {
        set_error(*ctx, ERR_ALIGNMENT_FAILURE,
                  "Alignment %llu exceeds page size %llu",
                  (unsigned long long)alignment,
                  (unsigned long long)getPageSize());
        return nil;
    }
    
    /* Simplified aligned allocation:
     * Allocate extra space and return aligned address within it.
     * Note: This is a simplified version. Production code would need
     * to store offset for proper freeing. */
    size_t extra = alignment + sizeof(BlockHeader);
    void* ptr = allocate_block(ctx, size + extra, nil, 0);
    if (!ptr) return nil;
    
    uintptr_t addr = (uintptr_t)ptr;
    uintptr_t aligned = ALIGN_UP(addr, alignment);
    
    /* If already aligned, return as-is */
    if (aligned == addr) {
        return ptr;
    }
    
    /* Return aligned address (Note: simplified - not production ready) */
    return (void*)aligned;
}

void std_free_diag(AllocatorContext* ctx, void* ptr) {
    free_block(ctx, ptr);
}

void std_free(AllocatorContext* ctx, void* ptr) {
    free_block(ctx, ptr);
}

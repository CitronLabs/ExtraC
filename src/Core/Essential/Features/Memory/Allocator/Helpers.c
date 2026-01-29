#include "Alloc.h"

/* === ERROR HANDLING === */

static const char* DefaultErrMessage[] = {
    [ERR.NONE] 				= "No error",
    [ERR.MEM.OUT_OF_MEMORY] 		= "Out of memory: system cannot allocate more pages",
    [ERR.MEM.METADATA_CAP_EXCEEDED] 	= "Metadata capacity exceeded: hit maxMetadataPages limit",
    [ERR.MEM.HEAP_LIMIT_REACHED] 	= "Heap limit reached: allocation would exceed maxSize",
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
    [ERR.MEM.LARGE_ALLOC_FAILED] 	= "Large allocation failed: direct page allocation error",
    [ERR.MEM.ALLOC_FAILED] 		= "Allocation failed: allocation error"
};

errvt moduleMethod(AllocCtx, recordError, errvt err) {
    
	if (err == ERR.MEM.CORRUPTION_DETECTED) {
		this.telemetry.corruptionEvents++;
	}
    
return ERR(err, generic DefaultErrMessage[err]);
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

return result;
}

len_t moduleMethod(AllocCtx, metadataSize){
	return  sizeof(BlockHeader) + 
		(this.settings.trackCallSites ? 
			sizeof(TrackingData) : 0) +
		(this.settings.useCanaries ?
			sizeof(u32) : 0);

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
    len_t temp = size - 1;
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
    FreeData* head 	= mod(parseBlock)(this.bin_offsets[idx]).free_data;
    
    block.free_data->next_free = this.bin_offsets[idx];
    block.free_data->prev_free = nil;
    
    if (head) {
        head->prev_free = block.header;
    }
    
    this.bin_offsets[idx] = block.header;
    block.header->magic   = MAGIC_FREE;
}

void moduleMethod(AllocCtx, Bin_remove, Block block) {

    len_t idx = mod(Bin_getIndex)(self, block.header->size);
    
    FreeData* prev = mod(parseBlock)(block.free_data->prev_free).free_data;
    FreeData* next = mod(parseBlock)(block.free_data->next_free).free_data;
    
    if (prev) {
        prev->next_free   = block.free_data->next_free;
    } else {
        this.bin_offsets[idx] = block.free_data->next_free;
    }
    
    if (next) {
        next->prev_free = block.free_data->prev_free;
    }
    
    block.free_data->next_free = nil;
    block.free_data->prev_free = nil;
}

/* === ALLOCATION STRATEGIES === */

Block moduleMethod(AllocCtx, findFit, len_t size) {
    /* Search bins starting from appropriate size class */
    loopat(i, mod(Bin_getIndex)(self, size), this.settings.binCount) {
        Block curr = mod(parseBlock)(this.bin_offsets[i]);
        
        while (curr.valid) {
            if (this.settings.validateOnEntry && !mod(isValidBlock)(self, curr.header, true)) {
                return (Block){0};
            }
            
            if (curr.header->size >= size) {
                return curr;
            }
            
            curr = mod(parseBlock)(curr.free_data->next_free);
        }
    }
    
return (Block){0};
}

/* === COALESCING === */

void moduleMethod(AllocCtx, coalesceBlock, Block blk) {
    /* Coalesce with next physical block if it's free */

    BlockHeader* header = blk.header;

    if (header->next != nil) {
        BlockHeader* next = header->next;
        
        if (next->is_free && mod(isValidBlock)(self, next, false)) {
            /* Remove next from free list */
            mod(Bin_remove)(self, mod(parseBlock)(next));
            
            /* Merge sizes */
            header->size += 	next->size;
            header->next = next->next;
            
            /* Update the block after next to point back to header */
            if (next->next != nil) {
                BlockHeader* next_next = next->next;
                next_next->prev   = header;
            }
            
            this.telemetry.coalescingEvents++;
        }
    }
    
    /* Coalesce with previous physical block if it's free */
    if (header->prev != nil) {
        BlockHeader* prev     = header->prev;
        Block 	     prev_blk = mod(parseBlock)(prev);

        if (prev->is_free && mod(isValidBlock)(self, prev, false)) {
            /* Remove prev from free list */
            mod(Bin_remove)(self, prev_blk);
            
            /* Merge sizes */
            prev->size += header->size;

            prev->next = header->next;
            
            /* Update the block after header to point back to prev */
            if (header->next != nil) {
                BlockHeader* next = header->next;
                next->prev = prev;
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

void moduleMethod(AllocCtx, allocLarge, 
	LargeAlloc* 	la, 
	len_t 		size, 
	len_t 		requested, 
	std_CodePos 	allocPos
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

errvt moduleMethod(AllocCtx, freeLarge, LargeAlloc* la) {
    LargeAlloc** curr = &this.large_allocs;
    
    /* Remove from list */
    *curr = (*curr)->next;
    
    /* Free tracking structure and memory */
    core.System.Mem.dealloc(la, la->size);
    
    this.telemetry.largeAllocations--;

return OK;
}

/* === HEAP GROWTH === */

errvt moduleMethod(AllocCtx, grow, len_t pages) {
    
    len_t pg_size  = core.System.Mem.getInfo().pageSize;
    len_t req_size = pages * pg_size;
    
    /* Check heap size limit */
    if (this.settings.maxSize > 0 && 
       (this.total_size + req_size > this.settings.maxSize)) {

	    // Check if we can grow to fit the maximum size
	    if(this.total_size >= this.settings.maxSize)
        	return mod(recordError)(self, ERR.MEM.HEAP_LIMIT_REACHED);
	    else
		req_size = this.settings.maxSize - this.total_size;
    }

    /* Allocate new pages */
    void* new_mem = core.System.Mem.alloc(req_size, pntr_shiftcpy(self, this.total_size));

    if (!new_mem) {
        errvt err = mod(recordError)(self, ERR.MEM.OUT_OF_MEMORY); printlnErr(
            "core.System.Mem.alloc failed to allocate ",
	    "pages for at least ",$(req_size)," bytes"
	);
        return err;
    }

    /* Check if new memory is contiguous */
    bool contiguous = (new_mem == pntr_shiftcpy(self, this.total_size));

    /* Handle contiguous requirement */
    if (this.settings.ensureContiguous && !contiguous) {
        len_t new_total = this.total_size + req_size;
        void* big_strip = core.System.Mem.alloc(new_total, nil);
        
        if (!big_strip) {
            core.System.Mem.dealloc(new_mem, req_size);

            errvt err = mod(recordError)(self, ERR.MEM.RELOCATION_FAILED); printlnErr(
                "Cannot allocate new memory for ensuring ",
		"a contiguous block of ",$(new_total)," bytes"
	    );
            return err;
        }

        /* Copy existing heap to new location */
        memcpy(big_strip, self, this.total_size);

        /* Update context */
        this.memory_object->pointer = big_strip;
        this.total_size 	    = new_total;

        /* Free old heap and unused new memory */
        core.System.Mem.dealloc(self, this.total_size);
        core.System.Mem.dealloc(new_mem, req_size);

	// Set new memory to end of heap
	new_mem = pntr_shiftcpy(self, this.total_size - req_size);
    } else {
        /* Just expand total size */
        this.total_size += req_size;
    }

    this.free_space = new_mem;

    /* Update telemetry */
    this.telemetry.pageCount += pages;
    this.telemetry.growthEvents++;

return OK;
}
Block moduleMethod(AllocCtx, newBlock, len_t needed) {

    len_t actual  = needed + mod(metadataSize)(self);
    len_t pg_size = core.System.Mem.getInfo().pageSize;

    if(!this.free_space){
	len_t new_space = 
		ALIGN_UP(MEM_ALLOC_GROWTH_FACTOR(this.total_size), pg_size) + 
		ALIGN_UP(needed, pg_size);

	if(this.settings.maxSize)
		new_space = new_space > this.settings.maxSize ? 
			ALIGN_UP(this.settings.maxSize, pg_size) : new_space;

	iferr(mod(grow)(self, new_space / pg_size))
		return (Block){};
    }

    Block new = {
	.valid    = true,
	.header   = this.free_space,
	.tracking = this.settings.trackCallSites ? 
		pntr_shiftcpy(this.free_space, sizeof(BlockHeader)) : nil
    };

    new.header->magic 		= MAGIC_FREE;
    new.header->canary_top 	= mod(getCanary)(self);
    new.header->requested_size 	= needed;
    new.header->size 		= actual;
    new.header->is_free 	= true;
    new.header->tracked 	= this.settings.trackCallSites;
    new.header->next 		= nil;
    new.header->prev 		= this.last_block;
    this.last_block->next 	= new.header;
    
    if(!this.settings.ensureContiguous && 
	new.header != pntr_shiftcpy(this.last_block, this.last_block->size)
    ){
	new.header->prev_contiguous      = false;
	this.last_block->next_contiguous = false;
    } else {
	new.header->prev_contiguous      = true;
	this.last_block->next_contiguous = true;
    }
    
    /* Add new block to free list */
    mod(Bin_insert)(self, new);

    if(this.settings.maxSize > 0 && 
       this.total_size + new.header->size < this.settings.maxSize
    )
    	pntr_shift(this.free_space, new.header->size);
    else
	this.free_space = nil;

    this.last_block = new.header;
    
return new;
}

/* === COMPLETE ALLOCATION IMPLEMENTATION === */

pntr moduleMethod(AllocCtx, allocateBlock, len_t size, std_CodePos allocPos){
    
    /* Validate size */
    if (size == 0) {
        mod(recordError)(self, ERR.MEM.INVALID_SIZE); printlnErr(
		"Allocation size is 0 at ", $use(std_CodePos_Type, &allocPos)
	);

        return nil;
    }
    
    /* Large allocation path - bypass normal allocator */
    if (this.settings.largeMmapThreshold > 0 && 
        size >= this.settings.largeMmapThreshold) {
        len_t 
	    pg_size    = core.System.Mem.getInfo().pageSize,
            alloc_size = ALIGN_UP(size, pg_size);
        
	pntr ptr = core.System.Mem.alloc(alloc_size, nil);

	if(ptr == nil){
		mod(recordError)(self, ERR.MEM.LARGE_ALLOC_FAILED);
		return nil;
	}
	
        mod(allocLarge)(self, ptr, alloc_size, size, allocPos);
        
        if (this.settings.enableTelemetry) {
            this.telemetry.activeAllocations++;
            this.telemetry.allocationCount++;
            this.telemetry.totalAllocated += size;
        }
        
        return ptr;
    }
    
    /* Calculate overhead for normal allocation */
    len_t overhead = sizeof(BlockHeader);
    if (this.settings.useCanaries) {
        overhead += sizeof(u32);  /* Bottom canary */
    }
    
    /* Check for overflow */
    if (size > maxof(len_t) - overhead) {
        mod(recordError)(self, ERR.MEM.OVERFLOW); printlnErr(
            "Allocation at ",	     $use(std_CodePos_Type, &allocPos),
	    " with Requested size ", $(size),
	    " + overhead ",	     $(overhead),
	    " exceeds maximum"
	);

        return nil;
    }
    
    len_t actual = ALIGN_UP(size + overhead, this.settings.alignment);
    if (actual < MIN_BLOCK_SIZE) {
        actual = MIN_BLOCK_SIZE;
    }
    
    Block chosen = {};
    
    chosen = mod(findFit)(self, actual);
    
    /* No suitable block found - try to grow heap */
    if (!chosen.valid) {
        chosen = mod(newBlock)(self, actual);

	if(!chosen.valid){
	    mod(recordError)(self, ERR.MEM.ALLOC_FAILED); printlnErr(
		"Failed to find a suitable block ",
		"and create a new block of size ", $(size),
		"at ", $use(std_CodePos_Type, &allocPos)
	    );
            return nil;
	}
    }
    
    /* Remove chosen block from free list */
    mod(Bin_remove)(self, chosen);
    
    /* Split block if remainder is large enough */
    if (chosen.header->size - actual >= this.settings.minSplitThreshold) {
        BlockHeader* remainder = pntr_shiftcpy(chosen.header, actual);
        
        remainder->magic 	  = MAGIC_FREE;
        remainder->canary_top 	  = mod(getCanary)(self);
        remainder->size 	  = chosen.header->size - actual;
        remainder->requested_size = 0;
        remainder->is_free 	  = true;
        remainder->next  	  = chosen.header->next;
        remainder->prev  	  = chosen.header;
        
        /* Update chosen block */
        chosen.header->next  = remainder;
        chosen.header->size 	  = actual;
        
        /* Update next block's prev pointer */
        if (remainder->next != nil) {
            BlockHeader* next 	= remainder->next;
            next->prev 	= remainder;
        }
        
        /* Add remainder to free list */
        mod(Bin_insert)(self, mod(parseBlock)(remainder));
        this.telemetry.splittingEvents++;
    }
    
    /* Finalize allocated block */
    chosen.header->magic 		= MAGIC_ALLOCATED;
    chosen.header->is_free 		= false;
    chosen.header->requested_size 	= size;
    
    if (this.settings.trackCallSites) {
	chosen.header->tracked 		= true;
        chosen.tracking->position	= allocPos;
    	chosen.tracking->alloc_num 	= ++this.alloc_counter;

	core.System.Time.getNow(
	    core.System.Time.Source.MONOTONIC, 
	    &chosen.tracking->time
	);
    }
    
    /* Set bottom canary if enabled */
    if (this.settings.useCanaries) {
        u32* bot_canary = pntr_shiftcpy(chosen.header, chosen.header->size - sizeof(u32));
        *bot_canary = chosen.header->canary_top;
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
return chosen.free_data;
}

/* === COMPLETE DEALLOCATION IMPLEMENTATION === */

errvt moduleMethod(AllocCtx, freeBlock, void* ptr, std_CodePos freePos) {
    nonull(ptr) return err; 


    // Get the magic 
    u32* magic = this.settings.trackCallSites ? 
		pntr_shiftcpy(ptr, -(sizeof(TrackingData) + sizeof(BlockHeader))) :
		pntr_shiftcpy(ptr, -sizeof(BlockHeader))
    ;
    
    /* Check if it's a large allocation */
    if (*magic == MAGIC_LARGE) {
        LargeAlloc* la 	= ptr;
        len_t requested = la->requested_size;
        
        if (mod(freeLarge)(self, ptr) == OK) {
            if (this.settings.enableTelemetry) {
                this.telemetry.activeAllocations--;
                this.telemetry.freeCount++;
                this.telemetry.totalAllocated -= requested;
            }
        }
        return OK;
    }
    elif (*magic == MAGIC_FREE) {
	BlockHeader*  freedBlock = generic magic;
	TrackingData* tracking   = this.settings.trackCallSites ? 
					pntr_shiftcpy(freedBlock, sizeof(BlockHeader)) : nil;

        errvt err = mod(recordError)(self, ERR.MEM.DOUBLE_FREE); 
	printlnErr("Double free detected at ",$(ptr));

	if(tracking)
	   printlnErr(", First free location: ", $use(std_CodePos_Type, &tracking->position));

	printlnErr(", Double free location: ", $use(std_CodePos_Type, &freePos));
        return err;
    }

    elif (*magic != MAGIC_ALLOCATED) {
        errvt err = mod(recordError)(self, ERR.MEM.INVALID_POINTER); printlnErr(
        	"Pointer ",$(ptr)," has invalid magic (not from this allocator)"
	);
        return err;
    }
    
    /* Normal allocation - get block header */
    Block block = mod(parseBlock)(generic magic);
    
    len_t payload_size = block.header->requested_size;
    
    /* Security features - zero or poison freed memory */
    if (this.settings.zeroOnFree && !this.settings.poisonOnFree) {
        memset(ptr, 0, payload_size);
    } else if (this.settings.poisonOnFree) {
        memset(ptr, 0xDE, payload_size);
    }
    
    /* Update telemetry */
    if (this.settings.enableTelemetry) {
	len_t overhead = 
		block.header->size - (mod(metadataSize)(self) + payload_size);

        this.telemetry.activeAllocations--;
        this.telemetry.freeCount++;
        this.telemetry.totalAllocated -= payload_size;
        this.telemetry.totalOverhead  -= overhead;
    }
    
    /* Mark as free and add to quarantine (which handles coalescing) */
    block.header->is_free = 1;
    block.header->magic   = MAGIC_FREE;

    if(block.header->tracked)
	block.tracking->position = freePos;

    mod(Quarantine_push)(self, block);

return OK;
}

/* === COMPLETE REALLOC IMPLEMENTATION === */
void* moduleMethod(AllocCtx, reallocBlock, void* ptr, len_t new_size, std_CodePos allocPos) {
    
    /* Standard realloc semantics */
    if (!ptr) {
        return mod(allocateBlock)(self, new_size, allocPos);
    }
    
    if (new_size == 0) {
        mod(freeBlock)(self, ptr, allocPos);
        return nil;
    }
    
    // Get the magic 
    u32* magic = this.settings.trackCallSites ? 
		pntr_shiftcpy(ptr, -(sizeof(TrackingData) + sizeof(BlockHeader))) :
		pntr_shiftcpy(ptr, -sizeof(BlockHeader))
    ;
    
    /* Check if it's a large allocation */
    if (*magic == MAGIC_LARGE) {
        /* For large allocations, just allocate new and copy */
        LargeAlloc* la = generic magic;
        len_t old_size = la->requested_size;
        
        void* new_ptr = mod(allocateBlock)(self, new_size, allocPos);
        if (new_ptr) {
            memcpy(new_ptr, ptr, MIN(old_size, new_size));
            mod(freeBlock)(self, ptr, allocPos);
            if (this.settings.enableTelemetry) 
                this.telemetry.reallocCount++;
            
        }
        return new_ptr;
    }
    elif (*magic == MAGIC_FREE) {
	BlockHeader*  freedBlock = generic magic;
	TrackingData* tracking   = this.settings.trackCallSites ? 
					pntr_shiftcpy(freedBlock, sizeof(BlockHeader)) : nil;

        mod(recordError)(self, ERR.MEM.DOUBLE_FREE); 
	printlnErr("Realloc Double free detected at ",$(ptr));

	if(tracking)
	   printlnErr(", First free location: ", $use(std_CodePos_Type, &tracking->position));

	printlnErr(", Realloc location: ", $use(std_CodePos_Type, &allocPos));
        return nil;
    }

    elif (*magic != MAGIC_ALLOCATED) {
        mod(recordError)(self, ERR.MEM.INVALID_POINTER); printlnErr(
        	"Pointer ",$(ptr)," has invalid magic (not from this allocator)"
	);
        return nil;
    }
    
    /* Normal allocation - get block header */
    Block block = mod(parseBlock)(generic magic);
    
    len_t old_size = block.header->requested_size;
    len_t overhead = mod(metadataSize)(self);
    
    len_t needed = ALIGN_UP(new_size + overhead, this.settings.alignment);
    if (needed < MIN_BLOCK_SIZE) {
        needed = MIN_BLOCK_SIZE;
    }
    
    /* If new size fits in current block, just update */
    if (needed <= block.header->size) {
        block.header->requested_size = new_size;
        if (this.settings.enableTelemetry) {
            this.telemetry.reallocCount++;
            this.telemetry.totalAllocated += (new_size - old_size);
        }
        return ptr;
    }
    
    /* Try expanding into next block if it's free */
    if (block.header->next != nil && block.header->next_contiguous) {
        Block next = mod(parseBlock)(block.header->next);
        
        if (next.free_data && (block.header->size + next.header->size >= needed)) {
            mod(Bin_remove)(self, next);
            
            len_t combined = block.header->size + next.header->size;
            block.header->size = combined;
            block.header->next = next.header->next;
            
            if (next.header->next != nil) {
                BlockHeader* next_next = next.header->next;
                next_next->prev = block.header;
            }
            
            block.header->requested_size = new_size;
            
            /* Update bottom canary */
            if (this.settings.useCanaries) {
                u32* bot = pntr_shiftcpy(block.header, block.header->size - sizeof(u32));
                *bot = block.header->canary_top;
            }
            
            if (this.settings.enableTelemetry) {
                this.telemetry.reallocCount++;
                this.telemetry.totalAllocated -= old_size;
		this.telemetry.totalAllocated += new_size;
                this.telemetry.coalescingEvents++;
            }
            
            return ptr;
        }
    }
    
    /* Need to allocate new block and copy */
    void* new_ptr = mod(allocateBlock)(self, new_size, allocPos);
    if (!new_ptr) {
        mod(recordError)(self, ERR.MEM.RELOCATION_FAILED); printlnErr(
                "Failed to allocate ",    $(new_size),
		" bytes for realloc at ", $use(std_CodePos_Type, &allocPos)
	);
        return nil;
    }
    
    memcpy(new_ptr, ptr, MIN(old_size, new_size));
    mod(freeBlock)(self, ptr, allocPos);
    
    if (this.settings.enableTelemetry) 
        this.telemetry.reallocCount++;
    
return new_ptr;
}

/* === WRAPPER FUNCTIONS === */
    /* Simplified aligned allocation:
     * Allocate extra space and return aligned address within it.
     * Note: This is a simplified version. Production code would need
     * to store offset for proper freeing. */
/*
void* std_aligned_alloc(AllocatorContext** ctx, len_t alignment, len_t size) {
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
    
    len_t extra = alignment + sizeof(BlockHeader);
    void* ptr = allocate_block(ctx, size + extra, nil, 0);
    if (!ptr) return nil;
    
    uintptr_t addr = (uintptr_t)ptr;
    uintptr_t aligned = ALIGN_UP(addr, alignment);
    
    // If already aligned, return as-is
    if (aligned == addr) {
        return ptr;
    }
    
    // Return aligned address (Note: simplified - not production ready)
    return (void*)aligned;
}
*/

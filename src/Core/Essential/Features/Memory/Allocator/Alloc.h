#pragma once
#include <XC.pkg.c>
#define module std, Memory, Allocator

/* === CONSTANTS === */

#define CANARY_VALUE_BASE    0xABADCAFE
#define POISON_VALUE         0xDEADBEEF
#define FREE_POISON          0xFEEEFEEE
#define MAGIC_ALLOCATED      0xA110CA7E
#define MAGIC_FREE           0xFEEEFEEE
#define MAGIC_LARGE          0x1A6EA110

/* === MACROS === */

#define PTR_TO_REL(base, ptr) \
    ((ptr) ? (pntrval)((u8*)(ptr) - (u8*)(base)) : -1)

#define REL_TO_PTR(base, rel) \
    ((rel) != -1 ? (pntr)((u8*)(base) + (rel)) : nil)

#define ALIGN_UP(val, align) \
    (((val) + ((align) - 1)) & ~((align) - 1))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define IS_POWER_OF_TWO(x) \
    (((x) != 0) && (((x) & ((x) - 1)) == 0))

#define MIN_BLOCK_SIZE (sizeof(FreeBlock) + sizeof(u32))

/* === TYPE DEFINITIONS === */
typedef struct {
    u32 	magic;                  /* Magic number for validation */
    u32 	canary_top;             /* Top canary for overflow detection */
    len_t 	size;                   /* Total block size including metadata */
    len_t 	requested_size;         /* Original user-requested size */
    pntrval 	rel_next_phys;          /* Relative pointer to next physical block */
    pntrval 	rel_prev_phys;          /* Relative pointer to previous physical block */
    
    bool 	is_free : 1;            /* 1 if free, 0 if allocated */
    bool 	tracked : 1;		/* if trackCallSites enabled for this allocation */
} BlockHeader;

typedef struct {
	std_ErrorPosition position;
	XC_Time 	  time;
	len_t 		  alloc_num;
} TrackingData;

typedef struct {
	pntrval rel_next_free;          	/* Relative pointer to next free block in bin */
	pntrval rel_prev_free;          	/* Relative pointer to previous free block in bin */
} FreeData;

typedef struct {
	BlockHeader* 	 header;
	TrackingData* 	 tracking;
	FreeData* 	 free_data;
	bool 		 valid;
} Block;

/* Large Allocation Tracking */
typedef struct LargeAlloc {
    void* 			ptr;            /* Pointer to allocated memory */
    len_t 			size;           /* Total size allocated */
    len_t 			requested_size; /* User-requested size */
    struct LargeAlloc* 		next;         	/* Next large allocation */
    struct LargeAlloc* 		prev;         	/* Prev large allocation */
} LargeAlloc;


/* Quarantine Node */
typedef struct QuarantineNode {
    void* 		   	block_ptr;   /* Pointer to BlockHeader */
    len_t 		   	size;        /* Block size */
    u64 		   	timestamp;   /* When quarantined */
    struct QuarantineNode* 	next;        /* Next in queue */
} QuarantineNode;

/* Quarantine Structure */
typedef struct Quarantine {
    QuarantineNode* 	head;           /* First node */
    QuarantineNode* 	tail;           /* Last node */
    len_t 		currentBytes;   /* Current quarantined bytes */
    len_t 		maxBytes;       /* Maximum quarantine size */
    len_t 		nodeCount;      /* Number of quarantined blocks */
} Quarantine;

/* Deferred Coalescing List */
typedef struct DeferredBlock {
    BlockHeader* block;              /* Block to coalesce */
    struct DeferredBlock* next;      /* Next deferred block */
} DeferredBlock;

typedef struct {
    std_Memory*		memory_object;	     	/* Memory Object header */
    
    mod(Settings)  	settings;         	/* Current settings */
    mod(Telemetry) 	telemetry;        	/* Statistics */
    
    len_t 		total_size;             /* Total memory allocated */
    len_t 		alloc_counter;          /* Monotonic allocation counter */
    u32   		canary_seed;            /* Seed for random canaries */
    
    Quarantine  	quarantine;          	/* Quarantine structure */
    LargeAlloc* 	large_allocs;        	/* List of large allocations */
    DeferredBlock* 	deferred_list;    	/* Deferred coalescing list */
    len_t 		deferred_count;         /* Count of deferred blocks */
    len_t 		deferred_threshold;     /* Threshold to trigger coalescing */
    
    pntrval 		bin_offsets[];          /* Flexible array of free list bins */
} AllocCtx;



constexpr int 
	Opt_Secure	= std_Memory_Allocator_Optimize_SECURITY,
	Opt_Speed 	= std_Memory_Allocator_Optimize_SPEED,
	Opt_Size  	= std_Memory_Allocator_Optimize_SIZE,
	Opt_SecureSpeed = std_Memory_Allocator_Optimize_SECURITY | std_Memory_Allocator_Optimize_SPEED,
	Opt_SecureSize  = std_Memory_Allocator_Optimize_SECURITY | std_Memory_Allocator_Optimize_SIZE,
	Opt_SpeedSize   = std_Memory_Allocator_Optimize_SPEED    | std_Memory_Allocator_Optimize_SIZE,
	Opt_Balanced    = std_Memory_Allocator_Optimize_SECURITY | 
			  std_Memory_Allocator_Optimize_SIZE 	 | 
			  std_Memory_Allocator_Optimize_SPEED;

#define UNLIMITED_SIZE 0

constexpr mod(Settings) 
	Setting_Secure      = {
    		.binCount 	 	= 32,
    		.alignment 	 	= 16,
    		.ensureContiguous  	= false,
    		.allowRuntimeTuning 	= true,
    		.enableTelemetry 	= true,
    		.maxSize 	 	= UNLIMITED_SIZE,  
    		.verboseErrors 	 	= true,
            	.minSplitThreshold 	= 64,
            	.zeroOnFree 		= true,
            	.poisonOnFree 		= false,
            	.useCanaries 		= true,
            	.validateOnEntry 	= true,
            	.useRandomCanaries 	= true,
            	.useQuarantine 		= true,
            	.quarantineSize 	= 1024 * 1024,
            	.useDeferredCoalescing 	= false,
            	.enableThreadCache 	= false,
            	.largeMmapThreshold 	= 256 * 1024,
            	.trackCallSites 	= true
	},
	Setting_Speed       = {
    		.binCount 	 	= 32,
    		.alignment 	 	= 16,
    		.ensureContiguous  	= false,
    		.allowRuntimeTuning 	= true,
    		.enableTelemetry 	= true,
    		.maxSize 	 	= UNLIMITED_SIZE,  
    		.verboseErrors 	 	= true,
            	.minSplitThreshold 	= 64,
            	.zeroOnFree 		= false,
            	.poisonOnFree 		= false,
            	.useCanaries 		= false,
            	.validateOnEntry 	= false,
            	.useRandomCanaries 	= false,
            	.useQuarantine 		= false,
            	.quarantineSize 	= 0,
            	.useDeferredCoalescing 	= true,
            	.enableThreadCache 	= false,
            	.largeMmapThreshold 	= 128 * 1024,
            	.trackCallSites 	= false
	},
	Setting_Size       = {
    		.binCount 	 	= 32,
    		.alignment 	 	= 16,
    		.ensureContiguous  	= false,
    		.allowRuntimeTuning 	= true,
    		.enableTelemetry 	= true,
    		.maxSize 	 	= UNLIMITED_SIZE,  
    		.verboseErrors 	 	= true,
            	.minSplitThreshold 	= 32,
            	.zeroOnFree 		= false,
            	.poisonOnFree 		= false,
            	.useCanaries 		= false,
            	.validateOnEntry 	= false,
            	.useRandomCanaries 	= false,
            	.useQuarantine 		= false,
            	.quarantineSize 	= 0,
            	.useDeferredCoalescing 	= false,
            	.enableThreadCache 	= false,
            	.largeMmapThreshold 	= 256 * 1024,
            	.trackCallSites 	= false
	},
	Setting_SecureSpeed = {
    		.binCount 	 	= 32,
    		.alignment 	 	= 16,
    		.ensureContiguous  	= false,
    		.allowRuntimeTuning 	= true,
    		.enableTelemetry 	= true,
    		.maxSize 	 	= UNLIMITED_SIZE,  
    		.verboseErrors 	 	= true,
	},
	Setting_SecureSize  = {
    		.binCount 	 	= 32,
    		.alignment 	 	= 16,
    		.ensureContiguous  	= false,
    		.allowRuntimeTuning 	= true,
    		.enableTelemetry 	= true,
    		.maxSize 	 	= UNLIMITED_SIZE,  
    		.verboseErrors 	 	= true,
	},
	Setting_SpeedSize   = {
    		.binCount 	 	= 32,
    		.alignment 	 	= 16,
    		.ensureContiguous  	= false,
    		.allowRuntimeTuning 	= true,
    		.enableTelemetry 	= true,
    		.maxSize 	 	= UNLIMITED_SIZE,  
    		.verboseErrors 	 	= true,
	},
	Setting_Balanced    = {
    		.binCount 	 	= 32,
    		.alignment 	 	= 16,
    		.ensureContiguous  	= false,
    		.allowRuntimeTuning 	= true,
    		.enableTelemetry 	= true,
    		.maxSize 	 	= UNLIMITED_SIZE,  
    		.verboseErrors 	 	= true,
            	.minSplitThreshold 	= 48,
            	.zeroOnFree 		= false,
            	.poisonOnFree 		= false,
            	.useCanaries 		= true,
            	.validateOnEntry 	= false,
            	.useRandomCanaries 	= false,
            	.useQuarantine 		= false,
            	.quarantineSize 	= 0,
            	.useDeferredCoalescing 	= false,
            	.enableThreadCache 	= false,
            	.largeMmapThreshold 	= 192 * 1024,
            	.trackCallSites 	= true
	};



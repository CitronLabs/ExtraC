#pragma once
#include "../types.h"

typedef struct {
	len_t 
	    pageSize,
	    pagesAlloced,
	    numNilAccess;
} XC_Memory_Info;

Interface(__XC_SYS_MEM,
	void* fn(alloc)(len_t num_pages);
	errvt fn(dealloc)(void* ptr, len_t num_pages);

	XC_Memory_Info fn(getInfo)();
	
	values(Perms, word,
		READ,
		WRITE,
		EXEC
	)
	errvt fn(protect)(
		void* ptr, 
		len_t num_pages, 
		errvt fn(fault_callback)(void*, len_t), 
		word permissions
	);
)

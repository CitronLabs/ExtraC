
#pragma once
#include "../types.h"

Interface(__XC_MEM,
	void* fn(alloc)(len_t num_pages);
	errvt fn(dealloc)(void* ptr, len_t num_pages);
	len_t fn(getPageSize)();
	
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

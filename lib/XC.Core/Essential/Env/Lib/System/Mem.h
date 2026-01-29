#pragma once
#include "../types.h"

typedef struct {
	len_t 
	    pageSize,
	    pagesAlloced;
} XC_Memory_Info;

Interface(__XC_SYS_MEM,
	void* fn(alloc)(len_t size, void* loc_hint);
	errvt fn(dealloc)(void* ptr, len_t size);

	XC_Memory_Info fn(getInfo)();
	
	values(Perms, word,
		READ,
		WRITE,
		EXEC
	)
	errvt fn(setViolationHandler)(errvt fn(fault_callback)(void*, len_t));
)

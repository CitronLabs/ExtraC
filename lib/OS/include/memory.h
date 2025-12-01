#pragma once
#include "./extern.h"


#define MEM_READ    1
#define MEM_WRITE   2
#define MEM_EXECUTE 4
#define MEM_COMMIT 8
#define MEM_SHARED  16


type(systemMemoryInfo,
    len_t total_Physical_Memory;
    len_t total_Virtual_Memory;
    len_t available_Physical_Memory;
    len_t available_Virtual_Memory;	
)

type(memoryInfo,
	u16 flags;
     	len_t size;
     	void* address;
	u8 committed : 1;
)

typedef void* memoryHandle;
Interface(memory,
	const strc8 stdVersion;
	errvt 		vmethod(initSystem);
	errvt 		vmethod(exitSystem);
	// the MEM_SHARED flag is set then at Address refers to the path where the shared memory object is going to be stored
	memoryHandle	vmethod(open,   	strc8 memObjPath);
	memoryHandle	vmethod(init,   	void* atAddress, len_t size, u16 flags);
	errvt		vmethod(map,   		memoryHandle handle, void* atAddress, len_t size, u16 flags);
	u64		vmethod(getSize,      	memoryHandle handle);
	errvt 		vmethod(setProt,   	memoryHandle handle, u16 flags);
	errvt 		vmethod(setSize,   	memoryHandle handle, len_t newSize);
	errvt 		vmethod(free,   	memoryHandle handle);
	errvt 		vmethod(watch,  	memoryHandle handle);
	errvt 		vmethod(setSwappable, 	memoryHandle handle, bool swappable);
	u32 		vmethod(getPageSize);
	errvt 		vmethod(getSysInfo,	systemMemoryInfo* info);
	errvt 		vmethod(getInfo,	memoryHandle handle, memoryInfo* info);
	void*		vmethod(getAddress,   	memoryHandle handle);
	bool		vmethod(isCommited,    	memoryHandle handle);
	errvt 		vmethod(commit,  	memoryHandle handle);
	namespace(dynlib,
	memoryHandle 	vmethod(load,   	strc8 path);
	void* 		vmethod(findSymbol,   	memoryHandle handle, strc8 symbol);
	errvt 		vmethod(unload, 	memoryHandle handle);
	errvt 		vmethod(handleEvents, 	memoryHandle handle, Queue(OSEvent) evntQueue);
	u64  	 	vmethod(pollEvents);
	)

)

#pragma once
#define __XC_OS_ENV_SYSTEM__
#include "../pkg.h"


#ifndef __XC_OS_ENV_SYSTEM_MEMORY__

#include "types.h"

#define package os_Env

Interface(Memory,
	values(Version, word,
		MAJOR,
		MINOR,
		PATCH
	)
	values(Flags, word,
		READ,
		WRITE,
		EXEC,
		COMMIT,
		SHARED
	)

	// the MEM_SHARED flag is set then at Address refers to the path where the shared memory object is going to be stored
	  
	MemoryHandle	fn(open)		(strc8 memObjPath);
	MemoryHandle	fn(init)		(void* atAddress, len_t size, u16 flags);
	errvt		fn(map)			(MemoryHandle handle, void* atAddress, len_t size, u16 flags);
	u64		fn(getSize)		(MemoryHandle handle);
	errvt 		fn(setProt)		(MemoryHandle handle, u16 flags);
	errvt 		fn(setSize)		(MemoryHandle handle, len_t newSize);
	errvt 		fn(free)		(MemoryHandle handle);
	errvt 		fn(watch)		(MemoryHandle handle);
	errvt 		fn(setSwappable)	(MemoryHandle handle, bool swappable);
	u32 		fn(getPageSize);	
	errvt 		fn(getSysInfo)		(MemorySysInfo* info);
	errvt 		fn(getInfo)		(MemoryHandle handle, MemoryInfo* info);
	void*		fn(getAddress)		(MemoryHandle handle);
	bool		fn(isCommited)		(MemoryHandle handle);
	errvt 		fn(commit)		(MemoryHandle handle);
	submodule(dynlib,
	MemoryHandle 	fn(load)		(strc8 path);
	void* 		fn(findSymbol)		(MemoryHandle handle, strc8 symbol);
	errvt 		fn(unload)		(MemoryHandle handle);
	errvt 		fn(handleEvents)	(MemoryHandle handle, ArrayQueue(OSEvent) evntQueue);
	u64  	 	fn(pollEvents);
	)

)

#undef package
#undef MemoryHandle
#undef MemorySysInfo
#undef MemoryInfo
#endif

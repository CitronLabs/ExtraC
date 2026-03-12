#pragma once
#define __XC_SYSTEM__
#include "../pkg.h"


#ifndef __XC_SYSTEM_MEMORY__
#define package std_System

type(Memory_Info, 
    len_t 
     	totalPhysical,
     	totalVirtual,
     	freePhysical,
     	freeVirtual
     ;
)

Class(Memory,
INIT(void* atAddress; len_t size; u16 flags),
FMT()
){
	values(Flags, word,
		READ,
		WRITE,
		EXEC,
		COMMIT,
		SHARED
	)

	pkg(Memory_Info) fn(getSysInfo)();

	errvt	method(Memory, map, void* atAddress, len_t size, u16 flags);
	errvt 	method(Memory, setProt, u16 flags);
	errvt 	method(Memory, setSize, len_t newSize);
	errvt 	method(Memory, watch);
	errvt 	method(Memory, setSwappable, bool swappable);
	u64	method(Memory, getSize);
	u16	method(Memory, getFlags);
	void*	method(Memory, getAddress);
	bool	method(Memory, isCommited);
	errvt 	method(Memory, commit);
};

#undef package
#endif

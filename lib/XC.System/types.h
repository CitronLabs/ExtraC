#pragma once
#define __XC_SYSTEM__
#include "pkg.h"

#define package std


Data(Module,
INIT(const c8* path),
FMT(),
     registerHandle handle;
);

type(System_Info,
	const c8* sysName,       implName;
     	uword 	  sysVersion[3], implVersion[3];
     	union {
	    struct {
		u64 moduleLoading : 1;
		u64 virtualMemory : 1;
		u64 multiThreads  : 1;
		u64 shellAccess   : 1;
     	    } field;
	    u64 bitField;
	} capabilities;
)


#undef package

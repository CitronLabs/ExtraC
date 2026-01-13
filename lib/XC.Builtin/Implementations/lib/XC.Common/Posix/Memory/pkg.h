#pragma once
#define __ENV_COMMON_POSIX__
#include "../pkg.h"



#ifndef __ENV_COMMON_POSIX_MEMORY__
	
#define package env_Common_Posix

Class(Dynlib,
INIT(strc8 path),
FMT(),
private(void* handle;)
){
	void* method(Dynlib, findSymbol, strc8 symbol);
};


Class(Memory, 
INIT(void* atAddress; len_t size; u16 flags), 
FMT(), 
	u16 userFlags;
     	void* address;
private(
	int fd;
	len_t size;
     	int prot, memFlags;
)
){
	values(Flag, word,
		READ,
		WRITE,
		EXECUTE,
		SHARE,
		COMMIT
	)
	errvt method(Memory, open, std_FSPath memObjPath);
	errvt method(Memory, setProt, u16 flags);
	errvt method(Memory, setSwappable, bool swappable);
	errvt method(Memory, commit);

	len_t fn(getPageSize)();

};
	



#undef package
#endif

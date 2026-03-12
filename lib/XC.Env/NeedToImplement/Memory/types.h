#pragma once
#define __XC_OS_SYSTEM_MEMORY__
#include "pkg.h"

#define package os_Sys_Memory

#define MemoryHandle 	os_Sys_Memory_Handle
#define MemorySysInfo 	os_Sys_Memory_SystemInfo
#define MemoryInfo 	os_Sys_Memory_Info

typefrom(pntr, Handle)

type(SystemInfo,
    len_t total_Physical_Memory;
    len_t total_Virtual_Memory;
    len_t available_Physical_Memory;
    len_t available_Virtual_Memory;	
)

type(Info,
	u16 flags;
     	len_t size;
     	void* address;
	u8 committed : 1;
)

#undef package

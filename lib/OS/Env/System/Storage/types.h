#pragma once
#define __XC_OS_ENV_SYSTEM_STORAGE__
#include "pkg.h"

#define package os_Env_Storage

#define StorageHandle os_Env_Storage_Handle
#define StorageDevice os_Env_Storage_Device

typefrom(pntr, Handle)

type(Device,
	std_String name;
	std_String manufacturer;
	std_String model;

	void* uniqueID;
	len_t  // The size in bytes of a single unit 
		// a.k.a KiB == 1,000, MiB = 100,000, etc
		measure,  
		exponant,	// storage_size = (measure * value)^exponant
		value;
)

#undef package
#define package os_Env_Storage_Filesys

typefrom(word, EventType);

type(Event,
    os_Env_Storage_Handle handle;
    os_Env_Storage_Filesys_EventType type;
)

#undef package

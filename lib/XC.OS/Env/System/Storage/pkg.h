#pragma once
#define __XC_OS_ENV_SYSTEM__
#include "../pkg.h"
#include "../User/pkg.h"


#ifndef __XC_OS_ENV_SYSTEM_STORAGE__

#include "types.h"

#define package os_Env

Interface(Storage,
  submodule(device,
	
  )
  submodule(Filesys,
	values(Event, word,
    		WRITE,
    		READ,
    		DELETE
	)
		errvt fn(readLink)    (std_FSPath path, std_FSPath result);
		errvt fn(makeLink)    (std_FSPath path, std_FSPath result);
		errvt fn(changePerms) (std_FSPath path, os_Env_User_Permissions perms);
	    	errvt fn(changeOwner) (std_FSPath path, os_Env_User_Handle user);
  );	

	i64 	 fn(write)	 (StorageHandle handle, pntr data, len_t size);
	i64 	 fn(read)	 (StorageHandle handle, pntr data, len_t size);
	errvt  	 fn(close)	 (StorageHandle handle);
	errvt  	 fn(handleEvents)(StorageHandle handle, ArrayQueue(OSEvent) evntQueue);
)

#undef package
#undef StorageHandle
#undef StorageDevice
#endif

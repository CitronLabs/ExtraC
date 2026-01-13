#pragma once
#define __ENV_WINDOWS__
#include "../pkg.h"


#ifndef __ENV_WINDOWS_RUNTIME__
#define package env_Windows

Interface(Runtime,
  submodule(Memory,
	
	void fn(init)();
  )
  submodule(Console,
	values(StreamType, word,
		OUTPUT,
		INPUT,
		ERR
	)

	HANDLE fn(getStream)(word);
	void fn(getInfo)(word);

	void fn(init)();
  )
  submodule(Device,
	errvt 				fn(init)();
	env_Common_Devices_Manager*     fn(getManager)();

	env_Common_Devices_ID fn(getIO)(), fn(getSys)();

	interface(device) IO, Sys;

	submodule(Resource,
		interface(stream)
	   		StdOut,
	   		StdIn,
	   		StdErr;

	   	interface(register)
	    		Locale, 
	    		WorkDir, 
	    		CliArgs;
	)
  )
	void fn(init)();
)

#undef package
#endif

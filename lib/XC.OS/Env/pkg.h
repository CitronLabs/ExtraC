#pragma once
#define __XC_OS__
#include "../pkg.h"


#ifndef __XC_OS_ENV__

#include "System/pkg.h"
#include "types.h"

Module(OSEnv){
	values(SystemID, word,
		GRAPHICS,
		AUDIO,
		INPUT,
		NETWORK,
		SCHEDULER,
		STORAGE
	)
	interface(os_Env_Input)    Input;
	interface(os_Env_Graphics) Graphics;
	interface(os_Env_Storage)  Storage;
	interface(os_Env_Network)  Network;
	interface(os_Env_User)     User;

	const char* fn(getName)();
	len_t       fn(pollEvents);

};

#undef package

#endif

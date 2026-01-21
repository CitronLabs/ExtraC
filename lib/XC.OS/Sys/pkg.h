#pragma once
#define __XC_OS__
#include "../pkg.h"


#ifndef __XC_OS_SYSTEM__

#include "Memory/pkg.h"
#include "Storage/pkg.h"
#include "Scheduler/pkg.h"
#include "Network/pkg.h"
#include "Input/pkg.h"
#include "Graphics/pkg.h"
#include "Audio/pkg.h"
#include "types.h"

Module(XC_Sys){
	values(SystemID, word,
		GRAPHICS,
		AUDIO,
		INPUT,
		NETWORK,
		SCHEDULER,
		STORAGE
	)
	interface(os_Sys_Input)    Input;
	interface(os_Sys_Graphics) Graphics;
	interface(os_Sys_Storage)  Storage;
	interface(os_Sys_Network)  Network;
	interface(os_Sys_User)     User;

	const char* fn(getName)();
	len_t       fn(pollEvents);

};

type(os_Event,
	union {
		os_Sys_Network_Socket_Event socket;
		os_Sys_Network_Object_Event network_object;
		os_Sys_Network_Device_Event network_device;

		os_Sys_Input_Event input;

		os_Sys_Graphics_Event 	graphics;

		os_Sys_Audio_Event audio;

		os_Sys_Storage_Filesys_Event filesys;

		os_Sys_Scheduler_Process_Event process;
     	}data;
	 
     	os_EventType
     	   		system,
     	   		resource;
)

#undef package

#endif

#pragma once
#define __XC_OS_ENV__
#include "pkg.h"
#include "System/pkg.h"

#define package os

typefrom(word, EventType);
typefrom(pntr, Handle);

type(Event,
	union {
		os_Env_Network_Socket_Event socket;
		os_Env_Network_Object_Event network_object;
		os_Env_Network_Device_Event network_device;

		os_Env_Input_Event input;

		os_Env_Graphics_Display_Event display;
		os_Env_Graphics_Video_Event video;

		os_Env_Audio_Event audio;

		os_Env_Storage_Filesys_Event filesys;

		os_Env_Scheduler_Process_Event process;
     	}data;
	 
     	os_EventType
     	   		system,
     	   		resource;
)


type(AppData,
	strc8  
     		appName, 
     		domainName;
)

#undef package

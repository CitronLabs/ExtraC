#pragma once
#define __XC_OS_SYSTEM__
#include "../pkg.h"


#ifndef __XC_OS_SYSTEM_AUDIO__

#include "Stream/pkg.h"
#include "Player/pkg.h"
#include "Processor/pkg.h"
#include "types.h"

#define package os_Sys

type(Audio_Device,
	std_String 
     		name,
		manufacturer,
		model;

	void* uniqueID;

     	pkg(Audio_DeviceType) type;
	pkg(Audio_Direction) direction;

	u32 currentSpec;
	pkg(Audio_Stream_Spec)* supportedSpecs;

     	union {
		pkg(Audio_Stream_Device) 	stream;
		pkg(Audio_Player_Device) 	player;
		pkg(Audio_Processor_Device) 	processor;
     	} info;
)

Interface(Audio,
	values(Version, word,
		MAJOR,
		MINOR,
		PATCH
	)

	values(ResourceID, word,
		DISPLAY,
		RENDER,
		VIDEO
	)
	errvt 			fn(initSystem)();
	errvt 			fn(exitSystem)();
	pkg(Audio_Handle) 	fn(grabDevice)(pkg(Audio_Device)* device);
	Array(AudioDevice) 	fn(enumDevices)();

	interface(pkg(Audio_Stream)) 	Stream;
	interface(pkg(Audio_Player)) 	Player;
	interface(pkg(Audio_Processor)) Processor;

	u64 		  	fn(pollEvents)();
	errvt 			fn(handleEvents)(pkg(Audio_Handle) handle, ArrayQueue(OSEvent) evntQueue);
)


#undef package
#undef AudioHandle 	
#undef AudioDirection 	
#undef AudioSpec 	
#undef AudioDevice 	
#endif

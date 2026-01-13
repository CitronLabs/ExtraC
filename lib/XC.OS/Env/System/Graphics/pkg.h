#pragma once
#define __XC_OS_ENV_SYSTEM__
#include "../pkg.h"


#ifndef __XC_OS_ENV_SYSTEM_GRAPHICS__

#include "Video/pkg.h"
#include "Display/pkg.h"
#include "Render/pkg.h"
#include "types.h"
#include "utils.h"

#define package os_Env

type(Graphics_Device,
	std_String 
     		name,
		manufacturer,
		model;

	void* uniqueID;

     	pkg(Graphics_DeviceType) type;
	pkg(Graphics_Direction) direction;

	u32 currentMode;
	pkg(Graphics_Video_Mode)* supportedModes;

     	union {
		pkg(Graphics_Video_Device) 	video;
		pkg(Graphics_Display_Device) 	display;
		pkg(Graphics_Render_Device) 	render;
     	} info;
)

Interface(Graphics,
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
	pkg(Graphics_Handle) 	fn(grabDevice)(pkg(Graphics_Device)* device);
	Array(GraphicsDevice) 	fn(enumDevices)();
	
	interface(pkg(Graphics_Display)) Display;
	interface(pkg(Graphics_Render)) Render;
	interface(pkg(Graphics_Video)) Video;

	errvt 		fn(handleEvents)(pkg(Graphics_Handle) handle, ArrayQueue(OSEvent) evntQueue);
	u64 		fn(pollEvents)	();
)

#undef package
#endif

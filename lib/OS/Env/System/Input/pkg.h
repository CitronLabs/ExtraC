#pragma once
#define __XC_OS_ENV_SYSTEM__
#include "../pkg.h"

#ifndef __XC_OS_ENV_SYSTEM_INPUT__

#include "types.h"

#define package os_Env

Interface(Input,
	values(Version, word,
		MAJOR,
		MINOR,
		PATCH
	)
	values(Type, word,
		KEY,
		POSITION,
		BUTTON
	)
	Array(InputDevice) fn(enumDevices)  ();
	errvt 		   fn(freeInput)    (InputHandle handle);
	InputHandle 	   fn(grabInput)    (InputDevice* dev, len_t index);
	errvt 		   fn(handleEvents) (InputHandle handle, ArrayQueue(OSEvent) evntQueue);
	u64 		   fn(pollEvents)   ();

	submodule(Position,
		
	)
	submodule(Key,

	)
	submodule(Button,

	)

	
)
#undef package
#undef InputHandle
#undef InputDevice
#undef InputInfo
#undef InputData
#undef Vec3D
#undef Vec2D
#undef Vec1D
#endif

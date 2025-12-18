#pragma once
#define __XC_OS_ENV_SYSTEM__
#include "../pkg.h"

#ifndef __XC_OS_ENV_SYSTEM_INPUT__

#include "types.h"

#define package os_Env

Interface(Input,
	const strc8 stdVersion;
	Array(InputDevice) fn(enumDevices)  ();
	errvt 		   fn(freeDevice)   (InputHandle handle);
	InputHandle 	   fn(grabDevice)   (InputDevice* dev);
	errvt 		   fn(handleEvents) (InputHandle handle, ArrayQueue(OSEvent) evntQueue);
	u64 		   fn(pollEvents);
)
#undef package
#undef KeyData
#undef KeyInput
#undef PosData
#undef PosType
#undef PosInput
#undef InputHandle
#undef Vec3D
#undef Vec2D
#undef Vec1D
#endif

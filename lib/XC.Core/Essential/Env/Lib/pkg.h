#pragma once
#define __XC_ENV__
#include "../pkg.h"

#ifndef __XC_ENV_LIB__
#include "utils.h"
#include "types.h"

#include "Device/Device.h"
#include "System/System.h"

Module(XC_Core){
	values(Version, word,
		MAJOR,
		MINOR,
		PATCH
	);

	interface(__XC_DEVICE) 	Device;
	interface(__XC_SYSTEM) 	System;
};



#endif

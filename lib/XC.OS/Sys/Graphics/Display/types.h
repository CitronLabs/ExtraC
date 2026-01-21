#pragma once
#define __XC_OS_SYSTEM_GRAPHICS_DISPLAY__
#include "pkg.h"

#define package os_Sys_Graphics_Display

type(Device,
	u16 dpi;
	u16 bitDepth;
	u16 rotation;
	
	bool primary;
)



#undef package

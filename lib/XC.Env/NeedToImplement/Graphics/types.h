#pragma once
#define __XC_OS_SYSTEM_GRAPHICS__
#include "pkg.h"


#define package os_Sys_Graphics
typefrom(pntr, Handle);

typefrom(bool, Direction);
typefrom(u32,  DeviceType);


typefrom(word, EventType);

type(Event,
    	pkg(Handle) handle;
 	pkg(EventType) type;
)

#undef package

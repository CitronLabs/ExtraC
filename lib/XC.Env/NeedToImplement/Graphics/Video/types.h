#pragma once
#define __XC_OS_SYSTEM_GRAPHICS_VIDEO__
#include "pkg.h"

#define package os_Sys_Graphics_Video


type(Mode,
	u32 width;
	u32 height;
	u16 refreshRate;
)

type(PixelFormat,
	u32 type;
     	u32 
     	    bottomMostModeIndex,
     	    topMostModeIndex;
)

type(Frame,
	pntr buffer;
     	u32  frameIndex;
)

type(Device,
	pkg(PixelFormat)* 	supportedPixFmts;
	u32 			currentPixFmt;
	bool 			primary;
)


#undef package

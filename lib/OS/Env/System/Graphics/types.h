#pragma once
#define __XC_OS_ENV_SYSTEM_GRAPHICS__
#include "pkg.h"

#define package os_Env_Graphics_Video

typefrom(bool, Direction);

#define FOURCC_CODE     os_Env_Graphics_Video_FOURCC_CODE
#define VideoMode 	os_Env_Graphics_Video_Mode
#define VideoDirection 	os_Env_Graphics_Video_Direction
#define VideoPxlFormat 	os_Env_Graphics_Video_PixelFormat
#define VideoFrame 	os_Env_Graphics_Video_Frame

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



#undef package
#define package os_Env_Graphics

#define GraphicsHandle os_Env_Graphics_Handle
#define GraphicsDevice os_Env_Graphics_Device

typefrom(pntr, Handle);

type(Device,
	std_String 
     		name,
		manufacturer,
		model;

	void* uniqueID;
     	
	VideoDirection direction;

	u32 currentMode;
	VideoMode* supportedModes;

	union{
	  struct{
		u16 dpi;
		u16 bitDepth;
		u16 rotation;
	
		bool primary;
	  } display;
	  struct{
		u32 currentPixFmt;
		VideoPxlFormat* supportedPixFmts;
	  } video;
	} info;
)

#undef package

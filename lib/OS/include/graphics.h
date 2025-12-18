#pragma once
#include "./extern.h"

type(VideoMode,
	u32 width;
	u32 height;
	u16 refreshRate;
)

#define FOURCC_CODE(code) ((u32)(code[0]) | ((u32)(code[1]) << 8) | ((u32)(code[2]) << 16) | ((u32)(code[3]) << 24))

typedef u32 FOURCC_CODE;
type(VideoPixelFormat,
	FOURCC_CODE type;
     	u32 
     	    bottomMostModeIndex,
     	    topMostModeIndex;
)

type(VideoFrame,
	void* buffer;
     	u32 frameIndex;
)
#define VIDEO_IN true
#define VIDEO_OUT false

typedef bool videoDirection;

type(graphicsDevice,
	inst(String) name;
	inst(String) manufacturer;
	inst(String) model;

	void* uniqueID;
     	
	videoDirection direction;

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
		VideoPixelFormat* supportedPixFmts;
	  } video;
	} info;


)
typedef void* graphicsHandle;


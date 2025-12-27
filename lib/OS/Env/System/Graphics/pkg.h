#pragma once
#define __XC_OS_ENV_SYSTEM__
#include "../pkg.h"
#include "../Input/pkg.h"


#ifndef __XC_OS_ENV_SYSTEM_GRAPHICS__

#include "types.h"
#include "utils.h"

#define package os_Env
#define Vec2D os_Env_Input_Vec2D

Interface(Graphics,
	values(Version, word,
		MAJOR,
		MINOR,
		PATCH
	)
	values(ResourceID, word,
		DISPLAY,
		VIDEO
	)
	errvt 			fn(initSystem);
	errvt 			fn(exitSystem);
	GraphicsHandle 		fn(grabDevice)(GraphicsDevice* device);
	Array(GraphicsDevice) 	fn(enumDevices)();

  submodule(Display,
	values(Event, word,
		RESIZE,
		CLOSE,
		VISIBILITY
	)
	GraphicsHandle 	fn(init)      	(Vec2D* pos, Vec2D* dim, GraphicsHandle parent);
	errvt	 	fn(close) 	(GraphicsHandle handle);
	errvt	 	fn(update)   	(GraphicsHandle handle, Vec2D* pos, Vec2D* dim, GraphicsHandle parent);
	bool	 	fn(isClosed) 	(GraphicsHandle handle);
	u64   		fn(pollEvents);

	GraphicsHandle 	fn(getDefault)();
	errvt 		fn(pushDefault)(GraphicsHandle handle);
	errvt 	     	fn(popDefault)(GraphicsHandle handle);
  )

  submodule(Video,
	values(Event, word,
		NEWFRAME
	)
	errvt		fn(start)	(GraphicsHandle handle);
	errvt		fn(stop)	(GraphicsHandle handle);
	errvt		fn(close)	(GraphicsHandle handle);
	errvt		fn(pullFrame)	(GraphicsHandle handle, VideoFrame* frame);
	errvt		fn(pushFrame)	(GraphicsHandle handle, VideoFrame* frame);
	u64   		fn(pollEvents);
  )

	errvt 		  	fn(handleEvents)(GraphicsHandle handle, ArrayQueue(OSEvent) evntQueue);
	u64 		  	fn(pollEvents);
)

#undef package
#define package os_Env_Graphics_Display

typefrom(word, EventType);

type(Event,
    	GraphicsHandle handle;
 	os_Env_Graphics_Display_EventType type;
)

#undef package
#define package os_Env_Graphics_Video

typefrom(word, EventType);

type(Event,
    	GraphicsHandle handle;
 	os_Env_Graphics_Video_EventType type;
)

#undef package
#undef Vec2D
#undef VideoMode 	
#undef VideoFrame	
#undef VideoDirection 	
#undef VideoPxlFormat 	
#undef GraphicsHandle 
#undef GraphicsDevice 
#undef FOURCC_CODE
#endif

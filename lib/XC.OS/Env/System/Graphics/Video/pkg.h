#pragma once
#define __XC_OS_ENV_SYSTEM_GRAPHICS__
#include "../pkg.h"


#ifndef __XC_OS_ENV_SYSTEM_GRAPHICS_VIDEO__
#include "../types.h"
#include "types.h"

#define package os_Env_Graphics

Interface(Video,
      values(Event, word,
      	NEWFRAME
      )
      errvt			fn(start)	(pkg(Handle) handle);
      errvt			fn(stop)	(pkg(Handle) handle);
      errvt			fn(close)	(pkg(Handle) handle);
      errvt			fn(pullFrame)	(pkg(Handle) handle, pkg(Video_Frame)* frame);
      errvt			fn(pushFrame)	(pkg(Handle) handle, pkg(Video_Frame)* frame);
      u64   			fn(pollEvents);
      Array(GraphicsDevice) 	fn(enumDevices)();
)

#undef package
#endif

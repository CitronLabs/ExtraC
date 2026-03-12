#pragma once
#define __XC_OS_SYSTEM_GRAPHICS__
#include "../pkg.h"


#ifndef __XC_OS_SYSTEM_GRAPHICS_DISPLAY__
#include "../types.h"
#include "types.h"
#define package os_Sys_Graphics

#define Vec2(name) const float name[restrict 2] 

Interface(Display,
	values(Event, word,
		RESIZE,
		CLOSE,
		VISIBILITY
	)
	pkg(Handle) 	fn(init)      	(Vec2(pos), Vec2(dim), pkg(Handle) parent);
	errvt	 	fn(close) 	(pkg(Handle) handle);
	errvt	 	fn(update)   	(pkg(Handle) handle, Vec2(pos), Vec2(dim), pkg(Handle) parent);
	bool	 	fn(isClosed) 	(pkg(Handle) handle);
	u64   		fn(pollEvents)	();
	
	pkg(Handle) 	fn(getDefault)	();
	errvt 		fn(pushDefault)	(pkg(Handle) handle);
	errvt 	     	fn(popDefault)	(pkg(Handle) handle);

)


#undef package
#undef Vec2
#endif

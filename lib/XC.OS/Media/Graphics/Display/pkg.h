#pragma once
#define __XC_OS_MEDIA_GRAPHICS__
#include "../pkg.h"

#ifndef __XC_OS_MEDIA_GRAPHICS_DISPLAY__
#define package os_Graphics

Class(Display, 
INIT(strc8 name; u64 w,h,x,y; pkg(Display)* parent),
FMT(),
	pkg(Display)* parent; 
      	std_String* name; 
      	len_t w,h,x,y;
){
      	errvt 		imethod(addChild, ifob(pkg(Display)) display);
      	errvt 		imethod(update);
      	errvt 		imethod(run);
      	errvt 		imethod(stop);
	bool 		imethod(isRunning);
	os_Handle  	imethod(getHandle);

	errvt  		imethod(getRenderer);
	errvt  		imethod(setRenderer);
	errvt		imethod(enumRenderers);

	interface(os_Media_Canvas_Handler) Canvas;
	os_Media_Stream* imethod(stream, os_Media_Stream* stream);

};





#undef package
#undef GFXColor 
#endif

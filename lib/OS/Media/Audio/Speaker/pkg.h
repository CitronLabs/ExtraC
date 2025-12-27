#pragma once
#define __XC_OS_MEDIA_AUDIO__
#include "../pkg.h"

#define package os

Class(Speaker,
INIT(),
FMT(),
){
      	errvt 		imethod(play);
      	errvt 		imethod(stop);
	bool 		imethod(isPlaying);
	os_Handle  	imethod(getHandle);

	interface(os_Media_Canvas_Handler) Canvas;
      	os_Media_Stream* method(Speaker, stream, os_Media_Stream* stream);

};


#undef package

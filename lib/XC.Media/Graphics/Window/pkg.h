#pragma once
#define __XC_MEDIA_GRAPHICS__
#include "../pkg.h"


#ifndef __XC_MEDIA_GRAPHICS_WINDOW__
#include "types.h"

#define package std

Class(Window,
INIT(),
FMT(),
){
	values(Event, uword,
		CLOSE,
		RESIZE,
		MOVE,
		FOCUS,
		UNFOCUS
	)

	errvt method(Window, setSize, len_t w, len_t h);
	errvt method(Window, setPosition, len_t x, len_t y);

	errvt method(Window, getInfo);

	errvt method(Window, run);
	errvt method(Window, stop);

      	errvt method(Window, update);
		
	bool method(Window, events);
	bool method(Window, isRunning);
};

#undef package
#endif

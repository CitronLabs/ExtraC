#pragma once
#define __XC_MEDIA__
#include "../pkg.h"
#include "../types.h"


#ifndef __XC_MEDIA_CANVAS__
#define package std_Media

Data(Canvas,
INIT(Type_t type; pntr data; len_t initLen),
FMT(),
	std_Buffer buffer;
);

Interface(Canvas_Handler,
	std_Media_Canvas*      	imethod(initCanvas, std_Media_Type type);
      	std_Media_Canvas* 	imethod(swapCanvas, std_Media_Canvas* canvas);
	errvt 			imethod(setCanvas,  std_Media_Canvas* canvas);
)

#undef package
#endif

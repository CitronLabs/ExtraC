#pragma once
#define __XC_OS_MEDIA__
#include "../pkg.h"
#include "../types.h"


#ifndef __XC_OS_MEDIA_CANVAS__
#define package os_Media

Class(Canvas,
INIT(os_Media_Type type; len_t initLen),
FMT(),
	os_Media_Type type;
private(
	std_Buffer* buffer;
	union {
		os_Media_Audio_Desc 	  audio;
      		os_Media_Vertex_Desc       vertex;
      		os_Media_Layout_Desc       layout;
      		os_Media_Color_Desc        color;
      		std_Object*		   	  other;
	} spec;

)
){
	values(Type, word,
		GENERIC,
		COLOR,
		AUDIO,
		VERTEX,
		LAYOUT
	)
};

Interface(Canvas_Handler,
	os_Media_Canvas*      	imethod(initCanvas, os_Media_Type type);
      	os_Media_Canvas* 	imethod(swapCanvas, os_Media_Canvas* canvas);
	errvt 			imethod(setCanvas,  os_Media_Canvas* canvas);
)

#undef package
#endif

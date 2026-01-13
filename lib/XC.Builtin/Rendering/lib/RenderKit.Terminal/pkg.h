#pragma once
#define __RENDERKIT__
#include "../RenderKit.pkg.h"

#define package renderkit

type(Terminal_Device,
	const std_String* name;		// terminal name
	const std_String* manufacturer;	// Windows, Linux, etc...
	const std_String* model;	// type of terminal

     	u16 
     	    colorDepth,
     	    maxWidth,
     	    maxHeight;
)

Class(Terminal,
INIT(
	std_Stream
      		* input, 
      		* output; 
	pkg(Terminal_Device) device;
),
FMT(),

){
	interface(os_Graphics_Display) Display;

	pkg(Terminal_Device) method(Terminal, getInfo);

};

#undef package

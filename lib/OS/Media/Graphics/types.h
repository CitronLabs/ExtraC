#pragma once
#define __XC_OS_MEDIA_GRAPHICS__
#include "pkg.h"

#define package os_Graphics

#define RGBA(r,g,b,a) (os_Graphics_Color){((float)r/255.0),((float)g/255.0),((float)b/255.0),((float)a/255.0)}

#define GFXColor 	os_Graphics_Color


type(Color, float r,g,b,a;);

Interface(ColorEncoding,
	GFXColor imethod(toRGBA);
	errvt 	 imethod(fromRGBA, GFXColor color);
)


#undef package

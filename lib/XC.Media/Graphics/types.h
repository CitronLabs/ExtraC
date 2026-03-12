#pragma once
#define __XC_OS_MEDIA_GRAPHICS__
#include "pkg.h"

#define package std_Graphics

#define RGBA(r,g,b,a) (std_Graphics_Color){((float)r/255.0),((float)g/255.0),((float)b/255.0),((float)a/255.0)}

type(Color, float r,g,b,a;);

Interface(ColorEncoding,
	pkg(Color) imethod(toRGBA);
	errvt 	   imethod(fromRGBA, pkg(Color) color);
)


#undef package

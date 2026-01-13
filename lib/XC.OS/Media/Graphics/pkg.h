#pragma once
#define __XC_OS_MEDIA__
#include "../pkg.h"
#include "../Canvas/pkg.h"
#include "../Stream/pkg.h"

#ifndef __XC_OS_MEDIA_GRAPHICS__

#include "Display/pkg.h"
#include "types.h"

#define package os

Interface(Graphics,
	interface(os_Graphics_Display) Display;
)

#undef package
#undef GFXColor 
#endif

#pragma once
#define __XC_MEDIA_GRAPHICS_WINDOW__
#include "pkg.h"

#define package std_Window

type(Event,
	uword type;

     	union {
	    struct { len_t x, y; } move;
	    struct { len_t w, h; } resize;
     	} data;
)

#undef package

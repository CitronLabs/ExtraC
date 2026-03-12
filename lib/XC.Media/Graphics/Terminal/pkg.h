#pragma once
#define __XC_MEDIA_GRAPHICS__
#include "../../pkg.h"


#ifndef __XC_MEDIA_GRAPHICS_TERMINAL__
#define package std

Class(Terminal,
INIT(),
FMT(),
     std_Stream stream;
){};

#undef package
#endif

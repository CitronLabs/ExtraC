#pragma once
#define __NO_BUILTIN__
#include <XC.pkg.h>

#ifndef __RENDERKIT__

#include "RenderKit.Terminal/pkg.h"
#include "RenderKit.GUML/pkg.h"

Module(renderkit){
	interface(renderkit_GUML) GUML;
};

#endif

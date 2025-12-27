#pragma once
#define __XC_OS__
#include "../pkg.h"
#include "../Env/types.h"


#ifndef __XC_OS_MEDIA__
	#include "types.h"

	#include "Canvas/pkg.h"
	#include "Stream/pkg.h"

	#include "Audio/pkg.h"
	#include "Graphics/pkg.h"

#define package os

	Interface(Media,
		interface(os_Media_Canvas) Canvas;
		interface(os_Media_Stream) Stream;
	)

#undef package
#endif

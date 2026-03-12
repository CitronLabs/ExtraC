#pragma once
#include <XC.Core/pkg.h>
#include <XC.Data/pkg.h>
#include <XC.Alloc/pkg.h>


#ifndef __XC_MEDIA__
#include "types.h"

#include "Canvas/pkg.h"
#include "Stream/pkg.h"

#include "Audio/pkg.h"
#include "Graphics/pkg.h"

#define package std

Interface(Media,
	interface(std_Media_Stream) Stream;
)

#undef package
#endif

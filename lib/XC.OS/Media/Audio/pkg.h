#pragma once
#define __XC_OS_MEDIA__
#include "../pkg.h"
#include "../Canvas/pkg.h"
#include "../Stream/pkg.h"

#ifndef __XC_OS_MEDIA_AUDIO__
#include "Player/pkg.h"
#define package os

Interface(Audio,
	interface(os_Audio_Player) Player;
)


#undef package
#endif

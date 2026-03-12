#pragma once
#define __XC_MEDIA__
#include "../pkg.h"
#include "../Canvas/pkg.h"
#include "../Stream/pkg.h"

#ifndef __XC_MEDIA_AUDIO__
#include "Player/pkg.h"
#define package std

Interface(Audio,
	interface(std_Audio_Player) Player;
)


#undef package
#endif

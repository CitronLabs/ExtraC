#pragma once
#define __XC_OS_ENV_SYSTEM_AUDIO__
#include "pkg.h"

#define package os_Env_Audio

#define AudioHandle 	os_Env_Audio_Handle
#define AudioDirection 	os_Env_Audio_Direction
#define AudioSpec 	os_Env_Audio_Spec
#define AudioDevice 	os_Env_Audio_Device

typefrom(pntr, Handle) // aka an audio stream

typefrom(u8, Direction);
typefrom(u32,  DeviceType);

typefrom(word, EventType)


type(Event,
	AudioHandle handle;
	os_Env_Audio_EventType type;
)

#undef package

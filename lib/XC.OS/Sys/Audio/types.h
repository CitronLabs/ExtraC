#pragma once
#define __XC_OS_SYSTEM_AUDIO__
#include "pkg.h"

#define package os_Sys_Audio

#define AudioHandle 	os_Sys_Audio_Handle
#define AudioDirection 	os_Sys_Audio_Direction
#define AudioSpec 	os_Sys_Audio_Spec
#define AudioDevice 	os_Sys_Audio_Device

typefrom(pntr, Handle) // aka an audio stream

typefrom(u8, Direction);
typefrom(u32,  DeviceType);

typefrom(word, EventType)


type(Event,
	AudioHandle handle;
	os_Sys_Audio_EventType type;
)

#undef package

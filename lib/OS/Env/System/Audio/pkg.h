#pragma once
#define __XC_OS_ENV_SYSTEM__
#include "../pkg.h"


#ifndef __XC_OS_ENV_SYSTEM_AUDIO__

#include "types.h"

#define package os_Env

Interface(Audio,
	values(Version, word,
		MAJOR,
		MINOR,
		PATCH
	)
	values(Event, word,
		NEWFRAME
	)
	errvt 			fn(initSystem)();
	errvt 			fn(exitSystem)();
	u64 		  	fn(pollEvents)();
	Array(AudioDevice) 	fn(enumDevices)();

  submodule(Stream,

	AudioHandle 		fn(grab)	(AudioDirection direction, AudioDevice* device, len_t framesize, AudioSpec spec);
	errvt			fn(start)	(AudioHandle handle);
	errvt			fn(stop)	(AudioHandle handle);
	errvt			fn(close)	(AudioHandle handle);
	errvt			fn(write)	(AudioHandle handle, void* buffer, len_t frames);
	errvt			fn(read)	(AudioHandle handle);
	errvt 		  	fn(handleEvents)(AudioHandle handle, ArrayQueue(os_Event) evntQueue);
	u64 		  	fn(pollEvents);
  )
)


#undef package
#undef AudioHandle 	
#undef AudioDirection 	
#undef AudioSpec 	
#undef AudioDevice 	
#endif

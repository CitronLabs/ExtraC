#pragma once
#define __XC_OS_ENV_SYSTEM_AUDIO__
#include "../pkg.h"


#ifndef __XC_OS_ENV_SYSTEM_AUDIO_STREAM__
#include "../types.h"
#include "types.h"

#define package os_Env_Audio

Interface(Stream,
  submodule(Flag,
	values(Resolution, u32,
		ANY,
		LOW,
		HIGH_REALTIME,
		HIGH_RENDER
      	)
	values(BufferType, u32,
		PCM,
		MODULATION,
		MIDI,
		ANALYSIS,
	    	SYNC
      	)
  )
      	errvt			fn(start)	(pkg(Handle) handle);
      	errvt			fn(stop)	(pkg(Handle) handle);
      	errvt			fn(close)	(pkg(Handle) handle);
      	u64   			fn(setCallback) (
					pkg(Handle) handle, 
					errvt fn(callback)(
						pkg(Handle) handle,
						pkg(Stream_Frame)* frame,
						Array(Stream_Frame*) aux_frames,
						void* user_info
					),
					void* user_info
				);
      	Array(AudioDevice) 	fn(enumDevices)();
)


#undef package
#endif



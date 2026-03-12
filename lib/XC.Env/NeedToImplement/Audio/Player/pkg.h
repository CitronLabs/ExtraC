#pragma once
#define __XC_OS_SYSTEM_AUDIO__
#include "../pkg.h"


#ifndef __XC_OS_SYSTEM_AUDIO_PLAYER__
#include "../types.h"
#include "types.h"
#define package os_Sys_Audio

Interface(Player,
	values(Event, word,
		CLOSE,
		UPDATE
	)
	values(Setting, word,
		SAMPLE_RATE,
		CHANNELS,
		BITS_PER_SAMPLE,
		PANNING,
		GAIN
	)

	errvt	 			fn(configure)  	(pkg(Handle) handle, pkg(Player_Settings)* settings);
	pkg(Player_Settings)	 	fn(info)  	(pkg(Handle) handle);
	bool	 			fn(exists) 	(pkg(Handle) handle);
	u64   				fn(pollEvents)	();
	
	pkg(Handle) 			fn(getDefault)	();
	errvt 				fn(setDefault)	(pkg(Handle) handle);

)


#undef package
#endif


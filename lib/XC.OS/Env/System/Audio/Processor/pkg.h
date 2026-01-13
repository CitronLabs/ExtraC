#pragma once
#define __XC_OS_ENV_SYSTEM_AUDIO__
#include "../pkg.h"


#ifndef __XC_OS_ENV_SYSTEM_AUDIO_PROCESSOR__
#include "../types.h"
#include "types.h"
#define package os_Env_Audio

Interface(Processor,
	Array(AudioDevice) 	fn(enumDevices)();
	
	/* --- Lifecycle --- */
	errvt fn(setup)(pkg(Handle) handle, pkg(Handle) display);
	errvt fn(destroy)(pkg(Handle) handle);

  submodule(Resource,
	values(Type, u32,
		BUFFER,
		SEQUENCE,
		STATE
	)
	values(Flag, u32,
		INPUT,
		OUTPUT,
		INPLACE,
		SCRATCH,
		VOLATILE
	)
    	pkg(Processor_ResourceID) 	fn(init)(pkg(Handle) handle,  u32 type_id, len_t size, u32 flags);
    	errvt 				fn(destroy)(pkg(Handle) handle, pkg(Processor_ResourceID) res);
    	
    	void* 				fn(map)(pkg(Handle) handle, pkg(Processor_ResourceID) res);
    	errvt 				fn(unmap)(pkg(Handle) handle, pkg(Processor_ResourceID) res);
    	
    	errvt 				fn(sync)(pkg(Handle) handle, pkg(Processor_ResourceID) res, len_t offset, len_t size);

    	errvt 				fn(commit)(pkg(Handle) handle, pkg(Processor_ResourceID) res);
  )
  submodule(Execution,
	values(Model, u32,
		REALTIME,
		BUFFERED,
		IN_PLACE,
		NON_REAL,
		INTERRUPT
	)
  submodule(Command,
	values(Player, u32,
		PLAY,
		PAUSE,
	    	SET_PARAM,
    		BYPASS	
	)
	values(Process, u32, 
	    	CLEAR_BUFFERS,
    		SET_PIPELINE_WIRE
	)
  )
	/* The Processing Core */
	// Returns the current sample-accurate hardware/host clock
	std_Clock* fn(getClock)(pkg(Handle) handle);
    
	// The main execution hook. In a DAW/ISR, the host calls this.
	// In a Player, the Player calls this to push to hardware.
	errvt fn(process)(pkg(Handle) handle, pkg(Processor_ResourceID) output);

	/* State & Parameter Control */
	errvt fn(execute)(pkg(Handle) handle, u32 command_id, void* payload);  
  )
)




#undef package
#endif

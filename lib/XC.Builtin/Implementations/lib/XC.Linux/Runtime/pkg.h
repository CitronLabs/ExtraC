#pragma once
#define __ENV_LINUX__
#include "../pkg.h"



#ifndef __ENV_LINUX_RUNTIME__
#define package env_Linux

Interface(Runtime,
  submodule(Devices,
	env_Common_Devices_Manager*     fn(getManager)();	
	env_Common_Devices_ID    	fn(getIO)();	
	env_Common_Devices_ID    	fn(getLocal)();	
	env_Common_Devices_ID    	fn(getSys)();	
  )

)

#undef package
#endif

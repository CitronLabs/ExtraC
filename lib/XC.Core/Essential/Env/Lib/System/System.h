#pragma once
#include "../types.h"

#include "./Locale.h"
#include "./Thread.h"
#include "./Time.h"
#include "./Mem.h"

typedef struct __xc_stateData* stateData;
typedef struct __xc_Process*   processHandle;
typedef struct __xc_Module*    moduleHandle;


Interface(__XC_SYSTEM,
      	errvt 		fn(command)(const char* command);
      	processHandle 	fn(execute)(const char* program, const char* arguments);
      	processHandle 	fn(thisProcess)();
      
      	moduleHandle 	fn(load)(const char* module);
      	pntr 		fn(find)(moduleHandle module, const char* symbol);
      	errvt 		fn(unload)(moduleHandle module);

	values(ExitCode, word,
		FAILURE,
	      	SUCCESS
	)
      	void fn(terminate)(processHandle process, word status, word flags); 

      	bool  fn(saveState)(stateData* state);
      	errvt fn(loadState)(stateData state);


	interface(__XC_SYS_LOCALE) 	Locale;
	interface(__XC_SYS_TIME) 	Time;
	interface(__XC_SYS_MEM) 	Mem;
	interface(__XC_SYS_THREAD) 	Thread;

)

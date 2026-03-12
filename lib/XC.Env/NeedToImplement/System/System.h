#pragma once
#include "../types.h"

#include "./Locale.h"
#include "./Thread.h"
#include "./Time.h"
#include "./Mem.h"
#include "./FS.h"

typedef struct __xc_stateData  	stateData;
typedef void*   		processHandle;


Interface(__XC_SYSTEM,
      	errvt 		fn(command)(const char* command);
      	processHandle 	fn(execute)(const char* program, const char* arguments);
      	processHandle 	fn(thisProcess)();
      
	values(ExitCode, word,
		FAILURE,
	      	SUCCESS
	)
      	void fn(terminate)(processHandle process, word status, word flags); 
      	void fn(waitFor)(processHandle process); 

      	bool  fn(saveState)(stateData* state);
      	errvt fn(loadState)(stateData state);


	interface(__XC_SYS_LOCALE) 	Locale;
	interface(__XC_SYS_TIME) 	Time;
	interface(__XC_SYS_MEM) 	Mem;
	interface(__XC_SYS_THREAD) 	Thread;
	interface(__XC_SYS_FS) 		FS;

)

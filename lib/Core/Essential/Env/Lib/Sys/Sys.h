#pragma once
#include "../types.h"

#include "./Locale.h"
#include "./Arch.h"

typedef struct __xc_stateData* stateData;


Interface(__XC_SYS,
      	errvt fn(execute)(const char* command);
      
	values(ExitCode, word,
		FAILURE,
	      	SUCCESS
	)
      	void fn(terminate)(word status, word flags); 

      	bool fn(saveState)(stateData* state);
      	errvt fn(loadState)(stateData state);

	interface(__XC_SYS_ARCH) 	Arch;
	interface(__XC_SYS_LOCALE) 	Locale;

)

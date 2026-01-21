#pragma once
#ifdef __ENV_NAME
#include <Env.pkg.h>

/*--------------------------------------|
 *					|
 *	   XC.System.Mem Module		|
 *					|
 *------------------------------------*/
#define module __ENV_NAME, XC, Core, System, Mem

moduleValues(Perms,
      	READ  , (1 << 0),
      	WRITE , (1 << 1),
      	EXEC  , (1 << 2),
);

export(
SUBMODULE(),
VALUES(Perms),
	alloc, getInfo, dealloc);

#undef module
/*--------------------------------------|
 *					|
 *	   XC.System.Time Module	|
 *					|
 *------------------------------------*/
#define module __ENV_NAME, XC, Core, System, Time


moduleValues(Source, 
	REALTIME,,
	MONOTONIC,,
)

export(
SUBMODULE(),
VALUES(),
	getNow, sleep
);

#undef module

/*--------------------------------------|
 *					|
 *	  XC.System.Locale Module	|
 *					|
 *------------------------------------*/
#define module __ENV_NAME, XC, Core, System, Locale

moduleValues(Temp,
	Celsius,,
	Fahrenheit,,
)

moduleValues(Measure,
	Metric,,
	Imperial,,
)

export(
SUBMODULE(),
VALUES(Temp, Measure),
);

#undef module

/*--------------------------------------|
 *					|
 *	     XC.System Module		|
 *					|
 *------------------------------------*/
#define module __ENV_NAME, XC, Core, System

moduleValues(ExitCode,
	FAILURE, -1,
	SUCCESS,  0,
)

export(
SUBMODULE(Mem, Locale, Time),
VALUES(ExitCode),
	saveState, loadState, terminate, execute
)


#undef module
#else
#error "__ENV_NAME must be defined in order to use this XC.Common shortcut"
#endif

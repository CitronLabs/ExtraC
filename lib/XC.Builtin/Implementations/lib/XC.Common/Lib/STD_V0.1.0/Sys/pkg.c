#pragma once
#ifdef __ENV_NAME
#include <Env.pkg.h>

/*--------------------------------------|
 *					|
 *	    XC.Sys.Mem Module		|
 *					|
 *------------------------------------*/
#define module __ENV_NAME, XC, Core, Sys, Mem

moduleValues(Perms,
      	READ  , (1 << 0),
      	WRITE , (1 << 1),
      	EXEC  , (1 << 2),
);

export(
SUBMODULE(),
VALUES(Perms),
	alloc, getInfo, dealloc, protect);

#undef module
/*--------------------------------------|
 *					|
 *	    XC.Sys.Time Module		|
 *					|
 *------------------------------------*/
#define module __ENV_NAME, XC, Core, Sys, Time


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
 *	    XC.Sys.Arch Module		|
 *					|
 *------------------------------------*/
#define module __ENV_NAME, XC, Core, Sys, Arch

export(
SUBMODULE(),
VALUES(),
	addCarry, subCarry, getFlags, 
	getFramePtr, getInstructionPtr, 
	getStackPtr, saveRegs, loadRegs,
	setFlags
);

#undef module

/*--------------------------------------|
 *					|
 *	   XC.Sys.Locale Module		|
 *					|
 *------------------------------------*/
#define module __ENV_NAME, XC, Core, Sys, Locale

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
 *	       XC.Sys Module		|
 *					|
 *------------------------------------*/
#define module __ENV_NAME, XC, Core, Sys

moduleValues(ExitCode,
	FAILURE, -1,
	SUCCESS,  0,
)

export(
SUBMODULE(Arch),
VALUES(ExitCode),
	saveState, loadState, terminate
)


#undef module
#else
#error "__ENV_NAME must be defined in order to use this XC.Common shortcut"
#endif

#pragma once
#ifdef __ENV_NAME
#include "Dev/pkg.c"
#include "Sys/pkg.c"

#define module __ENV_NAME, XC, Core

moduleValues(Version,
	MAJOR, 0,
	MINOR, 1,
	PATCH, 0
);


export(
SUBMODULE(Device, System),
VALUES(),
);

#undef module

#else
#error "__ENV_NAME must be defined in order to use this XC.Common shortcut"
#endif

#pragma once
#include "Env.pkg.h"
#include "XC.Common/pkg.c"

#if defined(__linux__) || defined(__linux) || defined(linux)
	#include "XC.Linux/pkg.c"
	#define OS_SUBMODULE Linux
#endif

#if defined(_WIN32) || defined(_WIN64)
	#include "XC.Windows/pkg.c"
	#define OS_SUBMODULE  Windows
#endif


#define module env

export(
SUBMODULE(OS_SUBMODULE),
VALUES(), 
);


#undef module

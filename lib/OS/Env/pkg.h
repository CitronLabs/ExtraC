#pragma once
#define __XC_OS__
#include "../pkg.h"


#ifndef __XC_OS_ENV__
#include "System/pkg.h"

Module(OSEnv){
	interface(os_Env_Input)    Input;
	interface(os_Env_Graphics) Graphics;

};

#undef package

#endif

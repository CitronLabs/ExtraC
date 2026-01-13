#pragma once
#define __ENV_COMMON_POSIX__
#include "../pkg.h"



#ifndef __ENV_COMMON_POSIX_THREAD__

#include "types.h"

#define package env_Common_Posix

Class(Thread,
INIT(),
FMT(),

){
    	interface(env_Common_Posix_Thread_Local) Local;

};

#undef package
#endif

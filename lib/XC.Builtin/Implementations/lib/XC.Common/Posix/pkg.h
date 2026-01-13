#pragma once
#define __ENV_COMMON__
#include "../pkg.h"
#include "../Devices/pkg.h"



#ifndef __ENV_COMMON_POSIX__

#include "types.h"
#include "IO/pkg.h"
#include "Thread/pkg.h"
#include "Memory/pkg.h"

#define package env_Common


Interface(Posix,
  interface(env_Common_Posix_IO) 	IO;
  interface(env_Common_Posix_Thread) 	Thread;
  interface(env_Common_Posix_Memory) 	Memory;
  interface(env_Common_Posix_Dynlib) 	Dynlib;
)


#undef package
#endif

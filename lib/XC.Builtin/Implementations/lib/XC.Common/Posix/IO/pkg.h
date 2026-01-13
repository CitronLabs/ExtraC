#pragma once
#define __ENV_COMMON_POSIX__
#include "../pkg.h"



#ifndef __ENV_COMMON_POSIX_IO__
	
#include "types.h"

#define package env_Common_Posix
	
Class(IO,
INIT(),
FMT(),
	deviceInfo info;      
){
	interface(env_Common_Posix_IO_File)	File;
	interface(env_Common_Posix_IO_Dir)	Dir;
};



#undef package
#endif

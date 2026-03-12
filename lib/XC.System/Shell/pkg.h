#pragma once
#define __XC_SYSTEM__
#include "../pkg.h"


#ifndef __XC_SYSTEM_SHELL__
#define package std_System

Class(Shell,
INIT(std_String *shell, *defaultArgs),
FMT()
){
	Process(Invoke, std_System_Shell,
		fn(command)(const char* command),
	 	fn(setOutput)(std_Stream* stream),
	 	fn(wait)();

	 	word fn(getExitCode)();
	)
};

#undef package
#endif

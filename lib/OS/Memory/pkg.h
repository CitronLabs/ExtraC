#pragma once
#define __XC_OS__
#include "../pkg.h"

#define package os
Class(DynamicLib, 
INIT(char* path), 
FMT(),
){
	void* method(DynamicLib, searchSymbol, char* symbol);
};

#undef package

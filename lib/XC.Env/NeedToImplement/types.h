#pragma once
#define __XC_OS_SYSTEM__
#include "pkg.h"

#define package os

typefrom(word, EventType);
typefrom(pntr, Handle);



type(AppData,
	strc8  
     		appName, 
     		domainName;
)

#undef package

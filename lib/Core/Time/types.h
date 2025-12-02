#pragma once

#define __XC_TIME__
#include "pkg.h"

#define package std

typefrom(word, Date_Format);
typefrom(i32, EventID);

type(Event, 
	std_EventID id;
     	void* message;
)

#undef package

#pragma once

#define __XC_FEATURE__

#include "../pkg.h"

#include "../Type/pkg.h"

#define package std


Class(Local,
INIT(len_t len),,
private(
	registerHandle local_register;
)){
	void* method(Local, getData);
};


#undef package

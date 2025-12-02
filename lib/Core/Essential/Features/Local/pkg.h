#pragma once

#define __XC_FEATURE__

#include "../pkg.h"

#include "../Type/pkg.h"

#define package std

#define Local(type) std_Local*

Class(Local,
INIT(len_t len; void* init),,
private(
	registerHandle local_register;
)){
	void* method(Local, getData);
};


#undef package

#pragma once
#define __XC_ESSENTIAL__
#include "pkg.h"


typedef unsigned int errvt;


typedef enum{ 
	#define __ERROR_CODES__
	#include "Features/Error/config.c"
	#undef __ERROR_CODES__
}XC_ERROR_CODES;

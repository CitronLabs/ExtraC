#pragma once
#define __XC_CORE__
#include "../pkg.h"

//=======EXPORTS========
#ifndef __XC_ESSENTIAL__
#define package std
	#include "utils.h"
	
	#include "Env/pkg.h"
	#include "Features/pkg.h"
#undef package
#endif

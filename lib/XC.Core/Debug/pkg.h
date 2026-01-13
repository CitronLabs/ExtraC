#pragma once
#include "../Essential/pkg.h"

//=======EXPORTS========
#ifndef __XC_DEBUG__
#define package std
	#include "types.h"
	#include "utils.h"
	
	#include "Logger/pkg.h"

	Interface(Debug,
		interface(std_Logger) Logger;
	)

#undef package
#endif




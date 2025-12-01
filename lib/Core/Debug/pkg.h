#pragma once
#include "../Essential/pkg.h"

Decl(String)

//=======EXPORTS========
#ifndef __XC_DEBUG__
#define package std
	#include "types.h"
	#include "utils.h"
	
	#include "Logger/pkg.h"

	Interface(__XC_DEBUG__,
		interface(std_Logger) Logger;
	)

#undef package
#endif




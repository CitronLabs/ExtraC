#pragma once
#include "../Essential/pkg.h"
#include "../Strings/pkg.h"
#include "../Alloc/pkg.h"


//=======EXPORTS========
#ifndef __XC_TIME__
#define package std

Decl(Time)
Decl(Date)
Decl(Watch)

	#include "types.h"
	#include "utils.h"
	
	#include "Time/pkg.h"
	#include "Clock/pkg.h"
	#include "Date/pkg.h"
	#include "Watch/pkg.h"

#undef package
#endif




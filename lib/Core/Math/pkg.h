#pragma once
#include "../Essential/pkg.h"


//=======EXPORTS========
#ifndef __XC_MATH__
#define package std

	#include "types.h"

	#include "Number/pkg.h"
	#include "Vector/pkg.h"

	Interface(__XC_Math__,
		interface(std_Number) Number;
		interface(std_Vector) Vector;

	)

#undef package
#endif

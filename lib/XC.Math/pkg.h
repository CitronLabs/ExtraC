#pragma once
#include <XC.Core/pkg.h>
#include <XC.Data/pkg.h>


//=======EXPORTS========
#ifndef __XC_MATH__
	#include "types.h"

	#include "Vector/pkg.h"

	#include "Algebra/pkg.h"
	#include "Calculus/pkg.h"
	#include "Complex/pkg.h"
	#include "Geometry/pkg.h"
	#include "Constants/pkg.h"
	#include "Statistics/pkg.h"

#define package std

Interface(Math,
	interface(std_Math_Algebra)    Algebra;
	interface(std_Math_Calculus)   Calculus;
	interface(std_Math_Complex)    Complex;
	interface(std_Math_Geometry)   Geometry;
	interface(std_Math_Constants)  Constants;
	interface(std_Math_Statistics) Statistics;

	values(Result, u32,
		EXPRESSION,
		VALUE,
		UNDEFINED,
		ERROR
	)
);

#undef package
#endif

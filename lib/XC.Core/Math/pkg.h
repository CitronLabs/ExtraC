#pragma once
#include "../Essential/pkg.h"


//=======EXPORTS========
#ifndef __XC_MATH__
	#include "types.h"

	#include "Number/pkg.h"
	#include "Vector/pkg.h"

	#include "Algebra/pkg.h"
	#include "Calculus/pkg.h"
	#include "Complex/pkg.h"
	#include "Geometry/pkg.h"
	#include "Constants/pkg.h"
	#include "Statistics/pkg.h"

#define package std

#define math(...) push(std_Math, (pntr[]){__VA_ARGS__})
#define MATH(...) new(std_Math, (pntr[]){__VA_ARGS__})

Class(Math,
INIT(pntr* expression),
FMT(),

){
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

	
};

#undef package
#endif

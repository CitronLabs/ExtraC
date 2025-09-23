#pragma once
#include "../types.h"
#include "../Strings/pkg.h"

Decl(Number)
Decl(Vector)

//=======EXPORTS========
#ifndef __XC_MATH__
	#include "types.h"
	#include "utils.h"
	
	#include "interfaces/SIMD.h"

	#include "classes/Number.h"
	#include "classes/Vector.h"
#endif

#pragma once
#include "../Essential/pkg.h"



//=======EXPORTS========
#ifndef __XC_STRINGS__
#define package std

Decl(String)
Decl(StringBuilder)

	#include "types.h"
	
	#include "StringBuilder/pkg.h"
	
#undef package
#endif

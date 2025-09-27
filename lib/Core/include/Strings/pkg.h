#pragma once
#include "../types.h"
#include "../Error/pkg.h"

Decl(StringBuilder);
Decl(String)

#include "../Data/pkg.h"

//=======EXPORTS========
#ifndef __XC_STRINGS__
	#include "types.h"
	#include "utils.h"
	
	#include "classes/String.h"
	#include "classes/StringBuilder.h"
#endif


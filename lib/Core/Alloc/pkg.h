#pragma once
#define __XC_CORE__
#include "../pkg.h"

#include "../Essential/pkg.h"


//=======EXPORTS========
#ifndef __XC_ALLOC__
#define package std_Alloc
	#include "types.h"
	#include "utils.h"
	
	#include "Allocator/pkg.h"

	#include "Buffer/pkg.h"
	#include "Arena/pkg.h"
	#include "Pool/pkg.h"


#undef package
#endif

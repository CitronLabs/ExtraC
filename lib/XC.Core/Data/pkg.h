#pragma once
#define __XC_CORE__
#include "../pkg.h"

#include "../Essential/pkg.h"
#include "../Alloc/pkg.h"
#include "../Math/pkg.h"


//=======EXPORTS========
#ifndef __XC_DATA__
#define package std
	#include "types.h"
	#include "utils.h"
	
	#include "List/pkg.h"
	#include "Map/pkg.h"
	#include "Struct/pkg.h"

	#include "DSN/pkg.h"


#undef package
#endif


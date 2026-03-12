#pragma once
#define __XC_CORE__
#include "../pkg.h"
#include "../utils.h"


//=======EXPORTS========
#ifndef __XC_TYPES__
	#include "Classes/pkg.h"

	#include "types.h"
	#include "utils.h"

	#include "XC/pkg.h"
	#include "Data/pkg.h"
	#include "Alloc/pkg.h"
	#include "Format/pkg.h"

#define package std

	Interface(Types,
		interface(std_Data_Ops)  data;
		interface(std_Alloc_Ops) alloc;
		interface(std_Format)    fmt;
	)

#undef package
#endif




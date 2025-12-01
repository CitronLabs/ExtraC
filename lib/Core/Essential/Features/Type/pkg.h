#pragma once
#define __XC_FEATURE__
#include "../pkg.h"

#include "../Classes/pkg.h"

//=======EXPORTS========
#ifndef __XC_TYPES__
#define package std
	#include "types.h"
	#include "utils.h"

	#include "Data/pkg.h"
	#include "Alloc/pkg.h"
	#include "Format/pkg.h"

	Interface(Type,
		interface(std_Data_Ops)  data;
		interface(std_Alloc_Ops) alloc;
	)

#undef package
#endif




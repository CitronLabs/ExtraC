#pragma once
#include "../types.h"

Decl(Buffer)
Decl(CMalloc)
Decl(Pool)
Decl(Arena)

//=======EXPORTS========
#ifndef __XC_ALLOC__
	#include "types.h"
	#include "utils.h"
	
	#include "interfaces/Allocator.h"

	#include "classes/Buffer.h"
	#include "classes/CMalloc.h"
	#include "classes/Arena.h"
	#include "classes/Pool.h"
#endif

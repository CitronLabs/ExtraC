#pragma once

#define __XC_MATH__
#include "pkg.h"

enum(numEquality,
     	NUM_NULL    = 0,
	NUM_EQUALS  = 1,
	NUM_GREATER = 2,
	NUM_LESSER  = 4,
	NUM_NOT     = 8
)

#define VECDATA_REG  0
#define VECDATA_LONG 1
#define VECDATA_BIG  2
#define VECDATA_VEC  3

struct(VecData,
     	u8 stack_allocated : 1;
	u8 floating : 1; 
	u8 datatype : 2; 
	size_t datasize, len; void* data;
)

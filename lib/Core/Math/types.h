#pragma once

#define __XC_MATH__
#include "pkg.h"

#define package std

typefrom(u32, Equality)

#define VECDATA_REG  0
#define VECDATA_LONG 1
#define VECDATA_BIG  2
#define VECDATA_VEC  3

type(VecData,
     	u8 stack_allocated : 1;
	u8 floating : 1; 
	u8 datatype : 2; 
	len_t datasize, len; void* data;
)

#undef package

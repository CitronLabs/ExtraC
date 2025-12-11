#pragma once
#define __XC_FEATURE_BASETYPE__
#include "../pkg.h"

#include "../XCTypes/pkg.h"

#define package std
Data(Object,,)

Data(Nil, 
INIT(),,
)


#define isImpl(func)  (func && func != nil)

typefrom(pntr, typeID)

#undef package

#pragma once
#define __XC_FEATURE_BASETYPE__
#include "../pkg.h"

#include "../XCTypes/pkg.h"

#define package std
Data(Object,,)

Data(Nil, 
INIT(),,
)

extern const void* __nil_obj;
len_t nilmethod();

#define nilmethod generic nilmethod

#define nil (generic __nil_obj)
#define nilobj(type) (type){0}

#define isNil(object) (object && object != nil)
#define isImpl(func)  (func && func != nilmethod)

#define null ((void*)0)

typefrom(pntr, typeID)

#undef package

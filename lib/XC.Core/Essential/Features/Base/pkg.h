#pragma once
#define __XC_FEATURE_BASETYPE__
#include "../pkg.h"

#include "../XCTypes/pkg.h"
#include "../Stream/pkg.h"

#define package std
Data(Object,,)

Data(Nil, 
INIT(),,
)


#define isImpl(func)  (func && func != nil)

typefrom(pntr, typeID)

Class(CLI,
INIT(),
FMT(),
     std_Stream stream;
){
	errvt method(CLI, getArg,  std_varData variable);
	errvt method(CLI, rewind,  len_t places);
	errvt method(CLI, advance, len_t places);
};


#undef package

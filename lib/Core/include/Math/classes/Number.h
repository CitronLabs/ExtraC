#pragma once

#define __XC_MATH__
#include "../pkg.h"
#include "../types.h"

#define N(number) new(Number,  #number)
#define n(number) push(Number, #number)

Class(Number,
__INIT(cstr initVal),
__FIELD(),
	interface(Formatter);
    	
	//Number Float Casts
      	errvt  method(Number, castToBigFloat);
      	double method(Number, castToLongFloat);
      	float  method(Number, castToFloat);

	//Number Int Casts
      	errvt  method(Number, castToBigInt);
      	u64    method(Number, castToLongInt);
      	u32    method(Number, castToInt);

	//Number Utils
	errvt  method(Number, setPrecision,,   u64 precision);
	bool   method(Number, isFloat);
	errvt  method(Number, setZero);
	
      	//Number Operations
 	errvt method(Number, AddInto,,      inst(Number) a, inst(Number) b);
	errvt method(Number, SubtractInto,, inst(Number) a, inst(Number) b);
	errvt method(Number, MultiplyInto,, inst(Number) a, inst(Number) b);
    	errvt method(Number, DivideInto,,   inst(Number) a, inst(Number) b, inst(Number) remainder);
 	
      	inst(Number) method(Number, Add,,      inst(Number) other);
	inst(Number) method(Number, Subtract,, inst(Number) other);
	inst(Number) method(Number, Multiply,, inst(Number) other);
    	inst(Number) method(Number, Divide,,   inst(Number) other, inst(Number) remainder);
    	
      	numEquality  method(Number, Compare,,  inst(Number) other);
);

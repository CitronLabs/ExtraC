#pragma once

#define __XC_MATH__
#include "../pkg.h"
#include "../types.h"

#define package std

#define N(number) new(Number,  #number)
#define n(number) push(Number, #number)

Decl(List)

Class(Number,
INIT(strc8 initVal),
FMT(byte base; bool floating),
private(
 	std_List* digits; 
	u32 exponent; // used only for floating point numbers
	int sign;
	u32 precision;
	u8 floating : 1;
)){
    	
	//Number Float Casts
      	errvt  method(Number, castToBigFloat);
      	double method(Number, castToLongFloat);
      	float  method(Number, castToFloat);

	//Number Int Casts
      	errvt  method(Number, castToBigInt);
      	u64    method(Number, castToLongInt);
      	u32    method(Number, castToInt);

	//Number Utils
	errvt  method(Number, setPrecision,   u64 precision);
	bool   method(Number, isFloat);
	errvt  method(Number, setZero);
	
      	//Number Operations
 	errvt method(Number, AddInto,      std_Number* a, std_Number* b);
	errvt method(Number, SubtractInto, std_Number* a, std_Number* b);
	errvt method(Number, MultiplyInto, std_Number* a, std_Number* b);
    	errvt method(Number, DivideInto,   std_Number* a, std_Number* b, std_Number* remainder);
 	
      	std_Number* method(Number, Add,      std_Number* other);
	std_Number* method(Number, Subtract, std_Number* other);
	std_Number* method(Number, Multiply, std_Number* other);
    	std_Number* method(Number, Divide,   std_Number* other, std_Number* remainder);
    	
      	std_numEquality  method(Number, Compare,  std_Number* other);
};

#undef package

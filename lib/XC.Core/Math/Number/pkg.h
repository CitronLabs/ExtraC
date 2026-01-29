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
	values(Equality, std_Math_Equality,
     		INVALID,    
		EQUALS,  
		GREATER, 
		LESSER,  
		NOT     
	)

	submodule(BigFloat,
 	    errvt method(Number, Add,      std_Number* other, std_Number* result);
	    errvt method(Number, Subtract, std_Number* other, std_Number* result);
	    errvt method(Number, Multiply, std_Number* other, std_Number* result);
    	    errvt method(Number, Divide,   std_Number* other, std_Number* remainder, std_Number* result);
	)

	submodule(BigInt,
 	    errvt method(Number, Add,      std_Number* other, std_Number* result);
	    errvt method(Number, Subtract, std_Number* other, std_Number* result);
	    errvt method(Number, Multiply, std_Number* other, std_Number* result);
    	    errvt method(Number, Divide,   std_Number* other, std_Number* remainder, std_Number* result);
	)
    	
	submodule(Cast,
      	    errvt  method(Number, toBigFloat);
      	    double method(Number, toLongFloat);
      	    float  method(Number, toFloat);

      	    errvt  method(Number, toBigInt);
      	    u64    method(Number, toLongInt);
      	    u32    method(Number, toInt);
	)

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
    	
      	std_Math_Equality method(Number, Compare,  std_Number* other);
};

#undef package

#pragma once
#include "BigFloat/pkg.c"
#include "BigInt/pkg.c"

#define module std, Number, Cast

export(,, 
      	 toBigFloat,
      	 toLongFloat,
      	 toFloat,

	
      	 toBigInt,
      	 toLongInt,
      	 toInt
)

#undef module
#define module std, Number

moduleValues(Equality,
     	INVALID,   0,  
	EQUALS,    1,
	GREATER,   2,
	LESSER,    4,
	NOT,       8,
)


export(
SUBMODULE(BigFloat, BigInt, Cast),
VALUES(Equality),
	
	 setPrecision,
	 isFloat,
	 setZero,
	
      	
 	AddInto,      
	SubtractInto, 
	MultiplyInto, 
    	DivideInto,   
 	
      	Add,      
	Subtract, 
	Multiply, 
    	Divide,   
    	
      	Compare  
)

#undef module





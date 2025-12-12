#pragma once
#define __XC_MATH__
#include "../pkg.h"

#define package std
	Decl(Number)

#undef package
#define package std_Math

typefrom(word, Constant_Type);
	
Interface(Constants,
  values(Value, double,
	PI, 			
	EULAR,			
	GOLDEN_RATIO,
	EULER_MASCH, 		
	PYTHAGORAS, 		
	NLOGOF2, 		
	GAUSS, 			
	GLAISHER_KINKELIN,
	APERY 			
  )
  values(Type, std_Math_Constant_Type,
	PI, 			
	EULAR,			
	GOLDEN_RATIO,
	EULER_MASCH, 		
	PYTHAGORAS, 		
	NLOGOF2, 		
	GAUSS, 			
	GLAISHER_KINKELIN,
	APERY 			
  )
	std_Number* fn(calculate)(std_Number* num, std_Math_Constant_Type type, len_t places);
)

#undef package

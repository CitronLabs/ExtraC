#pragma once
#include <XC.Core/pkg.h>


#ifndef __LABRAT_TEST__
#define package labrat

Class(UnitTest, 
INIT(strc8 name; 
     	Array(errvt fn()(UnitTest*)) procedures);
      	Array(UnitTest*)	     subtests
FMT(),
	ArrayList(UnitTest*) 	      subtests;
	ArrayList(UnitTest*) 	      procedures;
){
	bool  method(UnitTest, passed);
	errvt method(UnitTest, run);

	Process(Actions, pkg(UnitTest),
		fn(assert)(bool),
		fn(equals)(std_varData, std_varData),
		fn(differ)(std_varData, std_varData),
		fn(runSubtest)(strc8)
	 	;

	 	bool 
	 	fn(isModified)(pntr), 
	 	fn(isEqual)(std_varData, std_varData);

		pntr 
	 	fn(alloc)(Type_t*);
	)
};

#define assert(...) assert((__VA_ARGS__) ? OK : ERR(ERR.FAIL, "the assertion failed: " #__VA_ARGS__)) 

#undef package
#endif


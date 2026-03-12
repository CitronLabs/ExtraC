#include "../../Test.h"

errvt moduleFn(Map_test)(UnitTest* unit){

	try() UnitTests
	    	.start(unit)
	    	.runSubtest("XC.Core.Data.Map.Features")
	    	.runSubtest("XC.Core.Data.Map.Errors")
	    	.runSubtest("XC.Core.Data.Map.Cases")
	    	.end();
	catch 
		return err->errorcode;

return OK;
}

Tests moduleFn(Map_Features)();
Tests moduleFn(Map_Errors)();
Tests moduleFn(Map_Cases)();

Tests moduleFn(Map)(){
	static Tests tests = {};

	if(!tests.unit)
	tests.unit = new(UnitTest, 
		.name 		= "XC.Core.Data.Map",
		.procedures 	= arr(&mod(Map_test)),
		.subtests 	= arr(
			mod(Map_Features)().unit,
			mod(Map_Errors)().unit,
			mod(Map_Cases)().unit
		)
	);

return tests;
}

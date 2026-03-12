#include "../../Test.h"
#define module tests, Data

errvt moduleFn(List_test)(UnitTest* unit){

	try() UnitTests
	    	.start(unit)
	    	.runSubtest("XC.Core.Data.List.Features")
	    	.runSubtest("XC.Core.Data.List.Errors")
	    	.runSubtest("XC.Core.Data.List.Cases")
	    	.end();
	catch 
		return err->errorcode;

return OK;
}

Tests moduleFn(List_Features)();
Tests moduleFn(List_Errors)();
Tests moduleFn(List_Cases)();

Tests moduleFn(List)(){
	static Tests tests = {};

	if(!tests.unit)
	tests.unit = new(UnitTest, 
		.name 		= "XC.Core.Data.List",
		.procedures 	= arr(&mod(List_test)),
		.subtests 	= arr(
			mod(List_Features)().unit,
			mod(List_Errors)().unit,
			mod(List_Cases)().unit
		)
	);

return tests;
}

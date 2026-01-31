#include "../../Test.h"
#define module tests, Data

errvt moduleFn(DSN_test)(UnitTest* unit){

	try() UnitTests
	    	.start(unit)
	    	.runSubtest("XC.Core.Data.DSN.Features")
	    	.runSubtest("XC.Core.Data.DSN.Errors")
	    	.runSubtest("XC.Core.Data.DSN.Cases")
	    	.end();
	catch 
		return err->errorcode;

return OK;
}

Tests moduleFn(DSN_Features)();
Tests moduleFn(DSN_Errors)();
Tests moduleFn(DSN_Cases)();

Tests moduleFn(DSN)(){
	static Tests tests = {};

	if(!tests.unit)
	tests.unit = new(UnitTest, 
		.name 		= "XC.Core.Data.DSN",
		.procedures 	= arr(&mod(DSN_test)),
		.subtests 	= arr(
			mod(DSN_Features)().unit,
			mod(DSN_Errors)().unit,
			mod(DSN_Cases)().unit
		)
	);

return tests;
}

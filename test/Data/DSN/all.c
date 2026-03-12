#include <XC.Builtin/pkg.c>
#define module std, DSN

static Tests mod(Tests) = {};

errvt moduleFn(Test)(UnitTest* unit){

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

Tests moduleFn(Features)();
Tests moduleFn(Errors)();
Tests moduleFn(Cases)();

Tests moduleFn(DSN)(){

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

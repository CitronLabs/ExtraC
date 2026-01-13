#include "Test.h"
#define module std

errvt moduleFn(test)(UnitTest* test){

	try() UnitTests
	    	.start(test)
	    	.runSubtest("XC.Core")
	    	.end();
	catch 
		return err->errorcode;

return OK;
}


main(){

	Test.dependsOn(arr("XC_Env_Tests"));

	var unitTests = push(UnitTest, 
		.name 		= "XC.Core",
		.procedures 	= arr(&mod(test)),
		.subtests 	= tests.getAll(tests.Type.UNIT)
	);

return Test.Unit.run(unitTests);
}

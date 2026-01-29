#include "Test.h"
#define module std
            
            
            
            
            
            
            

errvt moduleFn(test)(UnitTest* test){

	try() UnitTests
	    	.start(test)
	    	.runSubtest("XC.Core.Features.Type")
	    	.runSubtest("XC.Core.Features.Memory")
	    	.runSubtest("XC.Core.Features.Arrays")
	    	.runSubtest("XC.Core.Features.Process")
	    	.runSubtest("XC.Core.Features.Stream")
	    	.runSubtest("XC.Core.Features.Error")
	    	.runSubtest("XC.Core.Features.String")

	    	.runSubtest("XC.Core.Data.List")
	    	.runSubtest("XC.Core.Data.Map")
	    	.runSubtest("XC.Core.Data.Struct")
	    	.runSubtest("XC.Core.Data.DSN")

	    	.runSubtest("XC.Core.Alloc.Buffer")
	    	.runSubtest("XC.Core.Alloc.Arena")
	    	.runSubtest("XC.Core.Alloc.Pool")

	    	.runSubtest("XC.Core.Time.Time")
	    	.runSubtest("XC.Core.Time.Clock")
	    	.runSubtest("XC.Core.Time.Date")
	    	.runSubtest("XC.Core.Time.Watch")

	    	.runSubtest("XC.Core.Debug.Logger")

	    	.runSubtest("XC.Core.Math.Number")

	    	.runSubtest("XC.Core.Strings.Editor")
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

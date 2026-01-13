#include "Test.h"
#define module std

errvt moduleFn(test)(UnitTest* test){

return OK;
}


main(){

	var unitTests = push(UnitTest, 
		.name 		= "Env",
		.procedures 	= arr(&mod(test)),
	);


return OK;
}

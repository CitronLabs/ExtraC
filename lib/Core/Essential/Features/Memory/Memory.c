#include "../../../pkg.h"

import(std)



construct(std_Memory,
FMT(),
DEF(),
	.Create = std_Memory_Op_Create	
){
	this.pointer = malloc(arg.size);
	this.size = arg.size;

	if(!this.pointer){
		ERR(ERR_FAIL, "failed to allocate memory");
		return nil;
	}

return self;
}

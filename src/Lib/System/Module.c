#include <Env.pkg.c>
#include <XC.pkg.c>
#define module std, Module

READ(std_Module){
	if(size % 2 != 0){
	    ERR(ERR.INVALID, "Invalid number of arguements provided");
	    return 0;
	}

	len_t symbolsRead = 0;

	loop(i, size){
	    strc8  symbol = data[i];
	    void** result = data[++i];

	    nonull(symbol, result) continue;

	    if(system.Register.Modify.readFrom(this.handle, &data[i], 0)) symbolsRead++;
	}
return symbolsRead;
}

construct(std_Module,
FMT(), 
DEF(),
	.Create = mod(Op_Create)
){
	nonull(arg.path) return nil;

	var memLoader = env.Devices.XC.Memory.Loader.getHandle();

	this.handle = system.Register.open(
		memLoader, 
		arg.path, 
		0, 0, 
		env.Devices.XC.Memory.Loader.Module
	);

	if(!this.handle){
		ERR(ERR.INIT, "Failed to load module");
		return nil;
	}

return self;
}

#include <Env.pkg.c>
#include <XC.pkg.c>
#define module std, Console

std_Console* moduleFn(getDefault)(){
	static std_Console defaultConsole = {};

	if(defaultConsole.__type == nil){
	   var consoleDevice = env.Devices.XC.IO.Console.getHandle(nil);
	   
	   var 
		stdOut = sys.Stream.fetch(consoleDevice, "StdOut", 0),
		stdIn  = sys.Stream.fetch(consoleDevice, "StdIn", 0),
		stdErr = sys.Stream.fetch(consoleDevice, "StdErr", 0);

	   	if(!create(std_Console, &defaultConsole, 
		    .output = new(std_Stream, std.Stream.Preset.fromHandle(stdOut)),
		    .input  = new(std_Stream, std.Stream.Preset.fromHandle(stdErr)),
	            .error  = new(std_Stream, std.Stream.Preset.fromHandle(stdErr)),
		    .freeOnDestroy = true
	    	)){
		    ERR(ERR.INIT, "Failed to initialize default console object");
		    return nil;
		}
	}

return &defaultConsole;
}



WRITE(std_Console){ return std.Types.data.writeTo(V(self->output), generic data, size); }
READ(std_Console){  return std.Types.data.readFrom(V(self->input), generic data, size); }


COPY(std_Console){
	std_Console* dest = where;

    try(){
	dest->__type = this.__type;
	dest->input  = copy(this.input,  new(std_Stream));
	dest->output = copy(this.output, new(std_Stream));
	dest->error  = copy(this.error,  new(std_Stream));

	privof(dest).freeOnDestroy = true;
		
    } catch {
	if(dest->input)  del(dest->input);
	if(dest->output) del(dest->output);
	if(dest->error)  del(dest->error);

	ERR(ERR.FAIL, "Failed to copy console info");
	return nil;
    }

return dest;
}

DESTROY(std_Console){
    if(priv.freeOnDestroy) del(
	this.input, 
	this.output, 
	this.error
    );

return OK;
}


construct(std_Console, 
FMT(), 
DEF(),
	.Create  = mod(Op_Create),
	.Destroy = mod(Op_Destroy),
	.Copy    = mod(Op_Copy),
	.Write   = mod(Op_Write),
	.Read    = mod(Op_Read),
){

	this.error  = arg.error;
	this.output = arg.output;
	this.input  = arg.input;

return self;
}

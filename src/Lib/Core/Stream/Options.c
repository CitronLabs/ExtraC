#include <XC.pkg.c>
#define module std, Stream_Options

from(std,
	Array_Stack    as Stack,
	Array_List     as List,
    	Stream_Options as Options,
    	use(Stream),
    	use(Process)
);

alias(core.Device.Stream, coreStream);

COPY(Options){ 
	if(!memcpy(where, self, sizeof(Options))){
		ERR(ERR.FAIL, "failed to copy stream options");
		return nil;
	}

return where;}

SET(Options){  
	if(!memcpy(self, value, sizeof(Options))) 
		return ERR(ERR.FAIL, "failed to set stream options");
return OK;}

SIZE(Options){ return sizeof(Options); }
DESTROY(Options){ return OK; }
PRINT(Options){

	return write(out,
		"(Stream_Options){ "
	      	".flags = { ", 
	      		this.flags.readOnly   ? "readOnly "   : fmt_skip,
	      		this.flags.writeOnly  ? "writeOnly "  : fmt_skip,
	      		this.flags.invalid    ? "invalid "    : fmt_skip,
	      	"}, ",
	      	".handle = ",        $use(pntr_Type, this.handle),    ", ",
	      	".initData = ",      $(this.init.data), 	", ",
	      	".initLen = ",       $(this.init.len), 	", ",
	      	" }",
	fmt_end);
}


construct(std_Stream_Options,
FMT(),
DEF(),
	.Create  = mod(Op_Create),
	.Destroy = mod(Op_Destroy),
	.Copy    = mod(Op_Copy),
	.Set     = mod(Op_Set),
	.Size    = mod(Op_Size),
	.Print   = mod(Op_Print),
){ passover }

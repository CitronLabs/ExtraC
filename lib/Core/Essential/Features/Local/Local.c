#include "../../../pkg.h"

import(std)
import(XC)

#define module std, Local

void* moduleMethod(std_Local, getData){
	devHandle local_dev = XC.Dev.stdHandle(XC.Dev.ID.Local);

return XC.Dev.Register.access(priv.local_register);
}

PRINT(std_Local){
	return write(out,
	    "(std_Local){ "
	      ".handle = ", $((void*)priv.local_register), ", ",
	      ".data = ", $(XC.Dev.Register.access(priv.local_register)), ", ",
	    " }",
	fmt_end);
}
SET(std_Local){
	memcpy(
	  XC.Dev.Register.access(priv.local_register), 
	  value,
	  XC.Dev.Register.info(priv.local_register).size
	);
return OK;
}
COPY(std_Local){
	memcpy(where, self, sizeof(std_Local));
return where;
}
DESTROY(std_Local){
	XC.Dev.Register.drop(priv.local_register);
return OK;
}
HASH(std_Local){
return pntr_asVal(priv.local_register);
}
SIZE(std_Local){
	return elements ? 
		XC.Dev.Register.info(priv.local_register).size :
	  	sizeof(std_Local)
	;
}

alias(XC.Dev.Register.Attrib, Attribs)

construct(std_Local,
FMT(),
DEF(),
	.Create  = std_Local_Op_Create,
	.Destroy = std_Local_Op_Destroy,
	.Size	 = std_Local_Op_Size,
	.Copy	 = std_Local_Op_Copy,
	.Hash	 = std_Local_Op_Hash,
	.Print	 = std_Local_Op_Print,
	.Set	 = std_Local_Op_Set
){
	if(!arg.len){
		ERR(ERR_INVALID, "cannot allocate 0 local bytes");
		return nil;
	}

	devHandle local_device = XC.Dev.stdHandle(XC.Dev.ID.Local);

	XC.Dev.Register.open(
			local_device, 
		      	Attribs->DIRECT_ACCESS,
		      	arg.len,
			nil
	);

}

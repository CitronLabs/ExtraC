#include <stdlib.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <sys/stat.h>

#include <XC/pkg.c>

from(env_Common_Posix,
     use(Dynlib)
)

void* moduleMethod(Dynlib, findSymbol, strc8 symbol) {
	nonull(self){ return nil; }

	void* result = dlsym(priv.handle, symbol);
	
	if(!result)
		ERR(ERR.INVALID, "could not find symbol");

return result;    
}

DESTROY(Dynlib){
	nonull(self){ return err; }

	if (dlclose(priv.handle) != 0) 
		return ERR(ERR.FAIL, "failed to close dynamic library");
return OK;
}


construct(env_Common_Posix_Dynlib,
FMT(), 
DEF(), 
){
	void* dynlib = dlopen(arg.path, RTLD_LAZY);

	if(!dynlib){
	    struct stat temp = {0};

	    if(stat(arg.path, &temp) == -1){
	    	ERR(ERR.INVALID, "invalid dynamic lib path");}
	    else
		ERR(ERR.FAIL, "failed to load dynamic lib");
	    
	    return nil;
	}

return self;
}

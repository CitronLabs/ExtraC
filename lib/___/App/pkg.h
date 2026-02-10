#pragma once
#include <XC.pkg.h>


#ifndef __XC_APP__

Module(app){
	values(Version, uword,
		MAJOR,
		MINOR,
		PATCH
	)

	submodule(Install,


	)
	submodule(Run,


	)
	submodule(Info,
		noFail fn(printHelp)(std_String* command);
		noFail fn(printVersion)();

	)

};

#endif

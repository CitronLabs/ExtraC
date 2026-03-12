#pragma once
#include <XC.pkg.h>

#ifndef __XC_APP__
#include "Manage/pkg.h"
#include "Develop/pkg.h"


Module(app){
	values(Version, uword,
		MAJOR,
		MINOR,
		PATCH
	)

  submodule(Execute,
	

  )
  submodule(Manage,


  )
  submodule(Develop,
    submodule(Env,

	errvt fn(init)();
    )
	errvt fn(build)(strc8 sourceDir);
	errvt fn(compile)(strc8 sourceFile);
  )


  submodule(Info,
	noFail fn(printHelp)(std_String* command);
	noFail fn(printVersion)();

  )

	errvt fn(init)();

};

#endif

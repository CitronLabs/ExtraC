#pragma once
#ifdef __ENV_NAME
#include <Env.pkg.h>

/*--------------------------------------|
 *					|
 *	   XC.Dev.Stream Module		|
 *					|
 *------------------------------------*/
#define module __ENV_NAME, XC, Core, Dev, Stream

moduleValues(ID,
	In  as 0,
	Out as 1,
	Err as 2,
);

moduleValues(Attrib,
      	READ  as (1 << 0), 
      	WRITE as (1 << 1)
);

export(
SUBMODULE(),
VALUES(Attrib, ID),
	open, fetch, close, stdHandle 
);


#undef module
/*--------------------------------------|
 *					|
 *	      XC.Dev Module		|
 *					|
 *------------------------------------*/
#define module __ENV_NAME, XC, Core, Dev


moduleValues(Attrib,
	      	PRIVATE,,
	      	PUBLIC,,
	);

moduleValues(ID,
     		IO,,
	      	Local,,
	      	Sys,,   
	);


importFn(open, fetch, stdHandle)

export(
SUBMODULE(Stream),
VALUES(Attrib, ID),
	open, fetch, stdHandle);

#undef module
#else
#error "__ENV_NAME must be defined in order to use this XC.Common shortcut"
#endif

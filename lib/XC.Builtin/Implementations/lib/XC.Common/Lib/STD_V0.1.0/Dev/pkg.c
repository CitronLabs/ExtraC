#pragma once
#ifdef __ENV_NAME
#include <Env.pkg.h>

/*--------------------------------------|
 *					|
 *      core.Device.Register Module	|
 *					|
 *------------------------------------*/
#define module __ENV_NAME, XC, Core, Device, Register

exportFrom(PATH(Type, ENV_VAR),
SUBMODULE(),
VALUES(),
	open, close, delete, watch, isModified, 
	info, writeTo, readFrom, setTo, access
)

exportFrom(Modify,
SUBMODULE(),
VALUES(),
	open, close, delete, watch, isModified, 
	info, writeTo, readFrom, setTo, access
)

exportFrom(Type,
SUBMODULE(ENV_VAR),
VALUES(),

)

moduleValues(ID,
	CliArgs  as 0,
	Locale   as 1,
	WorkDir  as 2,
);

moduleValues(Attrib,
      	READ  		as (1 << 0), 
      	WRITE 		as (1 << 1),
      	DIRECT_ACCESS 	as (1 << 2)
);

export(
SUBMODULE(),
VALUES(Attrib, ID),
	open, fetch, close, stdHandle, delete
);


#undef module
/*--------------------------------------|
 *					|
 *	core.Device.Stream Module	|
 *					|
 *------------------------------------*/
#define module __ENV_NAME, XC, Core, Device, Stream

exportFrom(PATH(Type, DIR),
SUBMODULE(),
VALUES(),
	open, close, delete, watch, isModified, 
	info, writeTo, readFrom, shift, sync, 
	flush, control
)

exportFrom(PATH(Type, FILE),
SUBMODULE(),
VALUES(),
	open, close, delete, watch, isModified, 
	info, writeTo, readFrom, shift, sync, 
	flush, control
)

exportFrom(Modify,
SUBMODULE(),
VALUES(),
	open, close, delete, watch, isModified, 
	info, writeTo, readFrom, shift, sync, 
	flush, control
)

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
	open, fetch, close, edit, stdHandle, delete
);


#undef module
/*--------------------------------------|
 *					|
 *	    core.Device Module		|
 *					|
 *------------------------------------*/
#define module __ENV_NAME, XC, Core, Device

moduleValues(Resource,
	      	Stream,,
	      	Register,,
		Device,,
	);

moduleValues(Attrib,
	      	PRIVATE,,
	      	PUBLIC,,
	      	STREAM_CREATE,,
	      	REGISTER_CREATE,,
	);

moduleValues(ID,
     		IO,,
	      	Local,,
	      	Sys,,   
	);


importFn(open, fetch, stdHandle)

export(
SUBMODULE(Stream, Register),
VALUES(Attrib, ID, Resource),
	open, close, fetch, stdHandle, info);

#undef module
#else
#error "__ENV_NAME must be defined in order to use this XC.Common shortcut"
#endif

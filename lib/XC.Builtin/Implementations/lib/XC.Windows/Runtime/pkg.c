#pragma once
#include <XC.pkg.h>
#include <XC.Common/pkg.c>

#define module env, Windows, Runtime, Device



exportFrom(IO,
SUBMODULE(),
VALUES(),
	fetch, info, close, delete, edit, open
)

exportFrom(Sys,
SUBMODULE(),
VALUES(),
	fetch, info, close, delete, edit, open
)

exportFrom(PATH(Resource, StdErr),
SUBMODULE(),
VALUES(),
	open, close, delete, edit,
	writeTo, readFrom, watch,  shift, sync, 
	control, info, flush, isModified
)

exportFrom(PATH(Resource, StdOut),
SUBMODULE(),
VALUES(),
	open, close, delete, edit,
	writeTo, readFrom, watch,  shift, sync, 
	control, info, flush, isModified
)

exportFrom(PATH(Resource, StdIn),
SUBMODULE(),
VALUES(),
	open, close, delete, edit,
	writeTo, readFrom, watch,  shift, sync, 
	control, info, flush, isModified
)

exportFrom(PATH(Resource, Locale),
SUBMODULE(),
VALUES(),
	open, close, delete, edit,
	writeTo, readFrom, setTo, watch,  
	access, info, isModified
)

exportFrom(PATH(Resource, WorkDir),
SUBMODULE(),
VALUES(),
	open, close, delete, edit,
	writeTo, readFrom, setTo, watch,  
	access, info, isModified
)

exportFrom(PATH(Resource, CliArgs),
SUBMODULE(),
VALUES(),
	open, close, delete, edit,
	writeTo, readFrom, setTo, watch,  
	access, info, isModified
)

exportFrom(Resource,
SUBMODULE(StdIn, StdOut, StdErr, Locale, CliArgs),
VALUES(),
)

export(
SUBMODULE(IO, Sys, Resource),
VALUES(),
	getManager, getIO, getSys, init
)

#undef module

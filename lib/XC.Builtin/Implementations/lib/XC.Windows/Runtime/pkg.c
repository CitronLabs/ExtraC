#pragma once
#include <XC.pkg.h>
#include <XC.Common/pkg.c>

#define module env, Windows, Runtime

exportFrom(PATH(Device, IO),
SUBMODULE(),
VALUES(),
	fetch, info, close, delete, edit, open
)

exportFrom(PATH(Device, Sys),
SUBMODULE(),
VALUES(),
	fetch, info, close, delete, edit, open
)

exportFrom(PATH(Device, Resource, StdErr),
SUBMODULE(),
VALUES(),
	open, close, delete, edit,
	writeTo, readFrom, watch,  shift, sync, 
	control, info, flush, isModified
)

exportFrom(PATH(Device, Resource, StdOut),
SUBMODULE(),
VALUES(),
	open, close, delete, edit,
	writeTo, readFrom, watch,  shift, sync, 
	control, info, flush, isModified
)

exportFrom(PATH(Device, Resource, StdIn),
SUBMODULE(),
VALUES(),
	open, close, delete, edit,
	writeTo, readFrom, watch,  shift, sync, 
	control, info, flush, isModified
)

exportFrom(PATH(Device, Resource, Locale),
SUBMODULE(),
VALUES(),
	open, close, delete, edit,
	writeTo, readFrom, setTo, watch,  
	access, info, isModified
)

exportFrom(PATH(Device, Resource, WorkDir),
SUBMODULE(),
VALUES(),
	open, close, delete, edit,
	writeTo, readFrom, setTo, watch,  
	access, info, isModified
)

exportFrom(PATH(Device, Resource, CliArgs),
SUBMODULE(),
VALUES(),
	open, close, delete, edit,
	writeTo, readFrom, setTo, watch,  
	access, info, isModified
)

exportFrom(PATH(Device, Resource),
SUBMODULE(StdIn, StdOut, StdErr, Locale, CliArgs),
VALUES(),
)

exportFrom(Device,
SUBMODULE(IO, Sys, Resource),
VALUES(),
	getManager, getIO, getSys, init
)

moduleValues(PATH(Console, StreamType),
	INPUT,,
	OUTPUT,,
	ERR,,
)

exportFrom(Console,
SUBMODULE(),
VALUES(),
	init, getInfo, getStream
)

exportFrom(Memory,
SUBMODULE(),
VALUES(),
	init 
)

export(
SUBMODULE(Console, Device, Memory),
VALUES(),
	init
)

#undef module

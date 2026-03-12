#pragma once
#include <XC.pkg.h>
#define module std, FS, Dir

moduleValues(PATH(Flags),
	WRITE  as 1,
	READ   as 2,
)

exportFrom(Async,
SUBMODULE(), 
VALUES(), 
	start, end, run, fail, result,

	cpy, move, rename, delete, createAt,
	addEntries, readEntries, waitFor, isDone
)

export(
SUBMODULE(Async), 
VALUES(Flags), 
	cpy, move, rename,
	update, delete, createAt, 
	addEntries, readEntries, getStream
)

#undef module

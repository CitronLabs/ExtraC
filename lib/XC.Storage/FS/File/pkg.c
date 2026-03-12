#pragma once
#include <XC.pkg.h>
#define module std, FS, File

moduleValues(PATH(Flags),
	WRITE  as 1,
	READ   as 2,
	APPEND as 4,
	CREATE as 8
)

exportFrom(Async,
SUBMODULE(), 
VALUES(), 
	start, end, run, fail, result,

	cpy, move, rename, delete, createAt,
	writeData, readData, writeFmt, readFmt,
	waitFor, isDone
)

export(
SUBMODULE(Async), 
VALUES(Flags), 
	cpy, move, rename,
	update, delete, createAt, 
	writeData, readData, getStream
)



#undef module

#pragma once
#include "File/pkg.c"
#include "Dir/pkg.c"
#define module std, FS

exportFrom(Path,
SUBMODULE(), 
VALUES(), 
	start, end, run, fail, result,

	join, setExtension, rename,
	move, chdir, resolve, getFileName,
	getExtension, isAbsolute, isRelative,
    	hasExtension, isChildOf
)


export(
SUBMODULE(File, Dir, Path), 
VALUES(), 
	getCurrent
)


#undef module

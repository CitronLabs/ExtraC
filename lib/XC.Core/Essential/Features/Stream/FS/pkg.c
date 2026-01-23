#pragma once
#include <XC.Core/pkg.h>
#define module std, FS



moduleValues(Flags,
	CREATE, 1, 
	APPEND, 2, 
	WRITE , 3,
	READ  , 4,
	DIR   , 5,
	LINK  , 6,
	RDWR  , std_FS_Flags_READ | std_FS_Flags_WRITE,
	
);

export(
SUBMODULE(),
VALUES(Flags),
	open, chdir, rename, search, 
	update, delete, cpy, move, temp
);


#undef module

#pragma once
#define module std, FS
#include <Core/pkg.h>

import(std)

importFn(open, chdir)

moduleValues(Flags,
	CREATE, 1, 
	APPEND, 2, 
	WRITE , 3,
	READ  , 4
);

export(
	Flags,
	open,
	chdir
);


#undef module

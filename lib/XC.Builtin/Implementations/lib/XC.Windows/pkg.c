#pragma once
#include <Env.pkg.h>

#include "Runtime/pkg.c"
#include "Lib/pkg.c"

#define module env, Windows


moduleValues(Version,
	MAJOR, 0,
	MINOR, 1,
	PATCH, 0,
)

export(
SUBMODULE(),
VALUES(Version), 
)

#undef module

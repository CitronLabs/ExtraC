#pragma once
#include <XC.Core/pkg.h>

#define module std, Types, alloc

export(
SUBMODULE(),
VALUES(),
	init, 	  initWith,
	destruct, destructWith
);

#undef module

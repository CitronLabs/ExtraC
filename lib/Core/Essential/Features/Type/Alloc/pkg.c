#pragma once
#include <Core/pkg.h>

#define module std, Types, alloc

importFn(init);

export(
SUBMODULE(),
VALUES(),
	init
);

#undef module

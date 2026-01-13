#pragma once
#include <XC.Core/pkg.h>

#define module std, Memory, Allocator
export(
SUBMODULE(),
VALUES(),
	
)

#undef module
#define module std, Memory

export(
SUBMODULE(Allocator),
VALUES(),
	copyTo, setTo, compare, getHeap
)

#undef module

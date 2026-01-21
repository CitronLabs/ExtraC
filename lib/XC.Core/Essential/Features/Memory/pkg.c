#pragma once
#include <XC.Core/pkg.h>

#define module std, Memory

exportFrom(PATH(Allocator, Interface),
SUBMODULE(),
VALUES(),
	isStatic, Alloc, Resize, Free, getBytesAlloced, setMax
)

exportFrom(Allocator,
SUBMODULE(Interface),
VALUES(),
	setup
)

export(
SUBMODULE(Allocator),
VALUES(),
	copyTo, setTo, compare, getHeap
)

#undef module

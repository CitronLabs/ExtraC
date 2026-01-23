#pragma once
#include <XC.Core/pkg.h>

#define module std, Memory

moduleValues(PATH(Allocator, Optimize),
	SPEED 		as (1 << 0),
	SIZE 		as (1 << 1),
	SECURITY 	as (1 << 2),
)

exportFrom(PATH(Allocator, Interface),
SUBMODULE(),
VALUES(),
	isStatic, Alloc, Resize, Free, getBytesAlloced, setMax
)

exportFrom(Allocator,
SUBMODULE(Interface),
VALUES(Optimize),
	setup
)

export(
SUBMODULE(Allocator),
VALUES(),
	copyTo, setTo, compare, getHeap
)

#undef module

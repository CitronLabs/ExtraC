#pragma once
#include <XC.pkg.h>

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

moduleValue(PATH(Allocator, defaultSettings)) = { __DEFAULT_ALLOC_SETTINGS };

exportFrom(Allocator,
SUBMODULE(Interface),
VALUES(Optimize, defaultSettings),
	setup,   optimizeSettings,
	getSettings, getTelemetry
)

export(
SUBMODULE(Allocator),
VALUES(),
	copyTo, setTo, compare, getHeap
)

#undef module

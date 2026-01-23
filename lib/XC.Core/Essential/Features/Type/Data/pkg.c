#pragma once
#include <XC.Core/pkg.h>

#define module std, Types, data

export(
SUBMODULE(),
VALUES(),
	readFrom, writeTo,
	setTo, getFrom,
	copyTo, sizeOf,
	indexOf, hashOf  
);

#undef module

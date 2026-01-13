#pragma once
#include <XC.Core/pkg.h>

#define module std, Number, BigInt


export(
SUBMODULE(),
VALUES(),
	Add, Subtract, Divide, Multiply
)

#undef module

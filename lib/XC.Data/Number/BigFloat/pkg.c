#pragma once
#include <XC.pkg.h>

#define module std, Number, BigFloat


export(
SUBMODULE(),
VALUES(),
	Add, Subtract, Divide, Multiply
)

#undef module

#pragma once
#include "./pkg.h"
#include "Essential/pkg.c"
#include "Data/pkg.c"
#include "Math/pkg.c"

#define module std

export(
SUBMODULE(Types, DSN, Math, Number, Error),
VALUES(),
	
);

alias(std.Error.Code, ERR);

#undef module

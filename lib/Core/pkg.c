#pragma once
#include "./pkg.h"
#include "Essential/Features/Type/pkg.c"
#include "Essential/Env/pkg.c"
#include "Data/pkg.c"

#define module std

exportModule(
SUBMODULE(Types, DSN),
VALUES(),
	
);

static const alias(std.Error.Code, ERR)

#undef module

#pragma once
#include "XC.pkg.h"
#include "XC.Env/pkg.c"
#include "XC.Core/pkg.c"
#include "XC.Data/pkg.c"
#include "XC.Debug/pkg.c"
#include "XC.Alloc/pkg.c"
#include "XC.Math/pkg.c"
#include "XC.Time/pkg.c"
#include "XC.IO/pkg.c"
#include "XC.Storage/pkg.c"
#include "XC.System/pkg.c"

#define module std

export(
SUBMODULE(Types, DSN, Math, Number, Error, System),
VALUES(),
	
);

#undef module

alias(std.Error.Code, ERR);



#pragma once
#include "XC.pkg.h"
#include "Env.pkg.c"
#include "XC.Core/pkg.c"
#include "XC.OS/pkg.c"
#ifndef __NO_BUILTIN__
#include "XC.Builtin/pkg.c"
#endif

alias(__XC_PATH.Core, core)
alias(__XC_PATH.Sys,  sys)


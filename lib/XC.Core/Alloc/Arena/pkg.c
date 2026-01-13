#pragma once
#include <XC.Core/pkg.h>

#undef std_Arena

#define module std, Arena

export(
SUBMODULE(),
VALUES(),
Alloc)


#undef module

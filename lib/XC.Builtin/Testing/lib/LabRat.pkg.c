#pragma once
#include "LabRat.pkg.h"

#define module labrat

export(
SUBMODULE(),
VALUES()
)

#ifndef NO_TEST_ALIAS
#include <XC.Core/pkg.c>

#define UT UnitTests

alias(labrat.Unit.Actions, UnitTests)
alias(labrat, 		   Test)

from(labrat,
	use(UnitTest)
)

typedef struct {
	UnitTest* unit;
} Tests;

#endif


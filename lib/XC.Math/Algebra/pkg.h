#pragma once
#define __XC_MATH__
#include "../pkg.h"
#include "../types.h"

#define package std_Math

Interface(Algebra,
  submodule(Linear,
			

  )

  pkg(Result) fn(solve)(pntr variable);
)

#undef package

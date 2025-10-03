#pragma once

#define __XC_ERROR__
#include "../pkg.h"

Interface(Loggable,
	errvt imethod(log,, inst(String) txt);
)

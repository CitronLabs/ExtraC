#pragma once
#include <Diesel.UAL/pkg.h>
#include "../utils.h"
#include "../config.c"

#if DIESEL_X86_SYNTAX ==  DIESEL_X86_SYNTAX_INTEL
	#include "Intel.h"
#elif DIESEL_X86_SYNTAX ==  DIESEL_X86_SYNTAX_ATT
	#include "AT&T.h"
#else
#error "Invalid x86 syntax type"
#endif

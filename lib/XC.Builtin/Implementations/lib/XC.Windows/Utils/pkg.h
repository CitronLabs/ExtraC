#pragma once
#define __ENV_WINDOWS__
#include "../pkg.h"



#ifndef __ENV_WINDOWS_RUNTIME__
#define package env_Windows

Interface(Utils,
	submodule(Error,
		strc16 fn(getMsg)(DWORD errorcode);
		errvt  fn(freeMsg)(strc16 errmsg);
	)
)

#undef package
#endif

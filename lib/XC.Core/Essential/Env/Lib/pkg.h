#pragma once
#define __XC_ENV__
#include "../pkg.h"

#ifndef __XC_ENV_LIB__
#include "utils.h"
#include "types.h"

#include "Dev/Dev.h"
#include "Sys/Sys.h"
#include "Vec/Vec.h"

Module(XC){
	values(Version, word,
		MAJOR,
		MINOR,
		PATCH
	);

	interface(__XC_DEVICE) 	Dev;
	interface(__XC_SYS) 	Sys;
};



#endif

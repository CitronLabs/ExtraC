#pragma once
#include <XC.Core/pkg.h>

#ifndef __LABRAT__

#include "LabRat.Unit/pkg.h"

Module(labrat){
	interface(labrat_UnitTest) Unit;

	noFail fn(dependsOn)(Array(strc8) names);
};

#endif



#pragma once

#define __XC_TIME__
#include "../pkg.h"
#include "../types.h"


Class(TimeWatch,
__INIT(inst(Time) limit; bool start),
__FIELD(inst(Time) start_time;),
	interface(Formatter);

    errvt method(TimeWatch, Start);
    inst(Time) method(TimeWatch, Check);
    inst(Time) method(TimeWatch, Stop);
)

#pragma once

#define __XC_TIME__
#include "../pkg.h"
#include "../types.h"


Class(TimeWatch,
INIT(inst(Time) limit; bool start),
FIELD(inst(Time) start_time;),

    errvt method(TimeWatch, Start);
    inst(Time) method(TimeWatch, Check);
    inst(Time) method(TimeWatch, Stop);
)

#pragma once

#define __XC_TIME__
#include "../pkg.h"
#include "../types.h"

Class(Time,
INIT(u64 seconds; u64 nanosec),
FIELD(u64 seconds; u64 nanosec),

	inst(Time) method(Time, GetNow,, TimeScale scale);
	errvt method(Time, Difference,, inst(Time) time2, inst(Time) result);
	bool  method(Time, Compare,, inst(Time) time2, inst(Time) tolerance);
	namespace(Utils,
	    namespace(sec,
		u64 vmethod(toMinutes, u64 sec);
		u64 vmethod(toHours,   u64 sec);
		u64 vmethod(toDays,    u64 sec);
		u64 vmethod(toMonths,  u64 sec);
		u64 vmethod(toYears,   u64 sec);
	    )	
	    namespace(nano,
		u64 vmethod(toMicro,   u64 nanosec);
		u64 vmethod(toMilli,   u64 nanosec);
		u64 vmethod(toSecond,  u64 nanosec);
	    )	
	)
)

#pragma once

#define __XC_TIME__
#include "../pkg.h"
#include "../types.h"

#define package std


struct TimeConversion {len_t result, remainder;};

Class(Time,
INIT(len_t seconds; len_t nanosec),
FMT(std_Date_Format type),
	len_t seconds; len_t nanosec;
){
	errvt method(Time, Difference, std_Time* time2, std_Time* result);
	bool  method(Time, Compare, std_Time* time2, std_Time* tolerance);
	submodule(Convert,
	    submodule(sec,
	      submodule(to,
		struct TimeConversion
		fn(minutes)(len_t sec),
		fn(hours)(len_t sec),
		fn(days)(len_t sec),
		fn(months)(len_t sec),
		fn(years)(len_t sec);
	      )
	      submodule(from,
		struct TimeConversion
		fn(minutes)(len_t sec),
		fn(hours)(len_t sec),
		fn(days)(len_t sec),
		fn(months)(len_t sec),
		fn(years)(len_t sec);
	      )
	    )	
	    submodule(nano,
	      submodule(to,
		struct TimeConversion
		fn(micro)(len_t nanosec),
		fn(milli)(len_t nanosec),
		fn(second)(len_t nanosec);
	      )
	      submodule(from,
		struct TimeConversion
		fn(micro)(len_t nanosec),
		fn(milli)(len_t nanosec),
		fn(second)(len_t nanosec);
	      )
	    )	
	)
};

#undef package

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
		struct TimeConversion
		fn(sec_to_minutes)(len_t sec),
		fn(sec_to_hours)(len_t sec),
		fn(sec_to_days)(len_t sec),
		fn(sec_to_months)(len_t sec),
		fn(sec_to_years)(len_t sec),

		fn(sec_from_minutes)(len_t sec),
		fn(sec_from_hours)(len_t sec),
		fn(sec_from_days)(len_t sec),
		fn(sec_from_months)(len_t sec),
		fn(sec_from_years)(len_t sec),

		fn(nano_to_micro)(len_t nanosec),
		fn(nano_to_milli)(len_t nanosec),
		fn(nano_to_second)(len_t nanosec),

		fn(nano_from_micro)(len_t nanosec),
		fn(nano_from_milli)(len_t nanosec),
		fn(nano_from_second)(len_t nanosec);
	)
};

#undef package

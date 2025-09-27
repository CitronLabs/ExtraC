#pragma once

#define __XC_TIME__
#include "pkg.h"

enum(TimeScale,
	TimeScale_UTC,
	TimeScale_TAI,
	TimeScale_TT,
	TimeScale_TDB,
	TimeScale_Relative,
)

struct(DateTime,
	u64 year;   u8  month; 
	u8  week;   u8  day;
	u8  hour;   u8  minute;
	u8  second; u8  timezone;
	u64 nanosec;
)
struct(JuilenTime,
	double time;
	double modified_time;
)

#pragma once

#define __XC_TIME__
#include "pkg.h"


// Note: These are for rough, user-friendly conversions.
// For precise date/time calculations, use a dedicated calendar library.
#define SECONDS_IN_MINUTE 60
#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_DAY 86400

#define SECONDS_IN_WEEK (7 * SECONDS_IN_DAY)
#define SECONDS_IN_MONTH (30.44 * SECONDS_IN_DAY) // Average
#define SECONDS_IN_YEAR (365.25 * SECONDS_IN_DAY) // Average with leap year

#define sec_to_min(sec) ((double)(sec) / SECONDS_IN_MINUTE)
#define sec_to_hour(sec) ((double)(sec) / SECONDS_IN_HOUR)
#define sec_to_day(sec) ((double)(sec) / SECONDS_IN_DAY)
#define sec_to_week(sec) ((double)(sec) / SECONDS_IN_WEEK)
#define sec_to_month(sec) ((double)(sec) / SECONDS_IN_MONTH)
#define sec_to_year(sec) ((double)(sec) / SECONDS_IN_YEAR)

#define nano_to_micro(nano) ((double)(nano) / 1000)
#define nano_to_millisec(nano) ((double)(nano) / 1000000)
#define nano_to_second(nano) ((double)(nano) / 1000000000)

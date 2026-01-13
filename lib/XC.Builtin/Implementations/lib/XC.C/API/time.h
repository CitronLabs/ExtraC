#pragma once
#define __XC_ENV_LIB_C__
#include "../pkg.h"

#include "stdlib.h"


#ifndef __XC_LIBC_HIDETYPES__

typedef size_t clock_t;
typedef size_t time_t;

struct tm {
   int tm_sec;   // Seconds after the minute (0-61)
   int tm_min;   // Minutes after the hour (0-59)
   int tm_hour;  // Hours since midnight (0-23)
   int tm_mday;  // Day of the month (1-31)
   int tm_mon;   // Months since January (0-11)
   int tm_year;  // Years since 1900
   int tm_wday;  // Days since Sunday (0-6)
   int tm_yday;  // Day of the year (0-365)
   int tm_isdst; // Daylight Saving Time flag (positive if DST in effect, 0 if not, negative if unknown)
};

#endif

Interface(C_TIME,
	char* 		fn(asctime)	(const struct tm *timeptr);
	clock_t 	fn(clock)	(void);
	char* 		fn(ctime)	(const time_t *timer);
	double 		fn(difftime)	(time_t time1, time_t time2);
	struct tm*	fn(gmtime)	(const time_t *timer);
	struct tm*	fn(localtime)	(const time_t *timer);
	time_t 		fn(mktime)	(struct tm *timeptr);
	size_t 		fn(strftime)	(char *str, size_t maxsize, const char *format, const struct tm *timeptr);
	time_t 		fn(time)	(time_t *timer);
	
	size_t 	 fn(wcsftime)(wchar_t *s, size_t maxsize, const wchar_t *format, const struct tm *timeptr);
)

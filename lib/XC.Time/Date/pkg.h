#pragma once

#define __XC_TIME__
#include "../pkg.h"
#include "../types.h"
#include "../Time/pkg.h"

#define package std



Class(Date, 
INIT(std_Time* time),
FMT(std_Date_Format type),
	len_t year;        
	len_t nanosec;
      	u32   offset_seconds;
      	u32   month  : 4; // 12 months fit into  2^4 = 16  
	u32   day    : 5; // 30-31 days fit into 2^5 = 32
	u32   hour   : 5; // 24 hours fit into 2^5 = 32
      	u32   minute : 6; // 60 minutes fit into 2^6 = 64
	u32   second : 6; // 60 seconds fit into 2^6 = 64   
      	// 4 + 5 + 5 + 6 + 6 = 26 bytes fits into a u32 bit integer w. 6 bytes spare 
){
	values(Format, word,
		US,
		Time_12Hour,
		Time_24Hour,
		EU,
		FullName,
		ISODate,
		ISOTime,
		RFC
	)
      	bool method(Date, isValid);
      	std_Time* method(Date, toTime);
      	const char* method(Date, getDayName, bool shortened);
      	const char* method(Date, getMonthName, bool shortened);
};


#undef package

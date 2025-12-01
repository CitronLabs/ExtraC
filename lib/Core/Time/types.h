#pragma once

#define __XC_TIME__
#include "pkg.h"

#define package std

enum(Date_Format,
	std_Date_Format_Null,
	std_Date_Format_US,
	std_Date_Format_Time_12Hour,
	std_Date_Format_Time_24Hour,
	std_Date_Format_EU,
	std_Date_Format_FullName,
	std_Date_Format_ISODate,
	std_Date_Format_ISOTime,
	std_Date_Format_RFC,
)

typefrom(i32, EventID);

type(Event, 
	std_EventID id;
     	void* message;
)

#undef package

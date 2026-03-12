#pragma once

#define __XC_TIME__
#include "../pkg.h"
#include "../types.h"

#define package std

Decl(Time)

Interface(ClockSystem,
	 std_Time* fn(getTime)(std_Time*);

)

Class(Clock, 
INIT(intf(std_ClockSystem) clockSystem), 
FMT(std_Date_Format type),
	intf(std_ClockSystem) system;
){
      	submodule(Systems,
	  submodule(UTC,
	     interface(std_ClockSystem) interface;
	     std_Clock* fn(getClock)();
	  )
	  submodule(TAI,
	     interface(std_ClockSystem) interface;
	     std_Clock* fn(getClock)();
	  )
	  submodule(TT,
	     interface(std_ClockSystem) interface;
	     std_Clock* fn(getClock)();
	  )
	)
	std_Time*  fn(getNow)();

	std_Time*  method(Clock, getTime, std_Time* time_buff);
	errvt 	   method(Clock, setPrimary);
	std_Clock* fn(getPrimary)();
};



#undef package

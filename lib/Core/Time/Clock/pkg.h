#pragma once

#define __XC_TIME__
#include "../pkg.h"
#include "../types.h"

#define package std

Decl(Time)

Interface(ClockSystem,
	std_Time* imethod(getTime);

)

Class(Clock, 
INIT(intf(std_ClockSystem) clockSystem), 
FMT(),

){
      	namespace(Systems,
	  namespace(UTC,
	     interface(std_ClockSystem) interface;
	     std_Clock* fn(getClock)();
	  )
	  namespace(TAI,
	     interface(std_ClockSystem) interface;
	     std_Clock* fn(getClock)();
	  )
	  namespace(TT,
	     interface(std_ClockSystem) interface;
	     std_Clock* fn(getClock)();
	  )
	)
	std_Time*  fn(getNow)();

	std_Time*  method(Clock, getTime);
	errvt 	   method(Clock, setPrimary);
	std_Clock* fn(getPrimary)();
};



#undef package

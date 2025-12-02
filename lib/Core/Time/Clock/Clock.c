#include "../../pkg.h"

import(std)
import(XC)

std_Time* vmethodimpl(std_Clock_System_UTC, getTime, std_Time* time_buff){
	timeSpec XCtime_buff;
	XC.Time.getNow(XC.Time.Source.REALTIME, &XCtime_buff);
	
	time_buff->seconds = XCtime_buff.seconds;
	time_buff->nanosec = XCtime_buff.nanoseconds;

return time_buff;
}

std_Clock* vmethodimpl(std_Clock_System_UTC,  getClock){
	static std_Clock clock = {0};

	if(!clock.system){
	    create(std_Clock, &clock, &std.Clock.Systems.UTC.interface);
	}

return &clock;
}

std_Time* vmethodimpl(std_Clock_System_TAI, getTime, std_Time* time_buff){
	timeSpec XCtime_buff;
	XC.Time.getNow(XC.Time.Source.MONOTONIC, &XCtime_buff);
	
	time_buff->seconds = XCtime_buff.seconds;
	time_buff->nanosec = XCtime_buff.nanoseconds;

return time_buff;
}

std_Clock* vmethodimpl(std_Clock_System_TAI,  getClock){
	static std_Clock clock = {0};

	if(!clock.system){
	    create(std_Clock, &clock, &std.Clock.Systems.TAI.interface);
	}

return &clock;
}

#define NUMBER_OF_MILLISEC_IN_MICROSEC (100 * 100 * 100)
#define TAI_TO_TT_INTEGER_OFFSET 32
#define TAI_TO_TT_DECIMAL_OFFSET (184 * NUMBER_OF_MILLISEC_IN_MICROSEC)

std_Time* vmethodimpl(std_Clock_System_TT, getTime, std_Time* time_buff){
	timeSpec XCtime_buff;
	XC.Time.getNow(XC.Time.Source.MONOTONIC, &XCtime_buff);
	
	len_t carry = 
		(XCtime_buff.nanoseconds + TAI_TO_TT_DECIMAL_OFFSET) / 1000000000;
			
	time_buff->nanosec = (XCtime_buff.nanoseconds + TAI_TO_TT_DECIMAL_OFFSET) % 1000000000 ;
	time_buff->seconds = XCtime_buff.seconds +  TAI_TO_TT_INTEGER_OFFSET + carry;

return time_buff;
}

std_Clock* vmethodimpl(std_Clock_System_TT,  getClock){
	static std_Clock clock = {0};

	if(!clock.system){
	    create(std_Clock, &clock, &std.Clock.Systems.TT.interface);
	}

return &clock;
}


std_Time*  vmethodimpl(std_Clock, getNow){
	Local(std_Time) time_buff = null;
	
	if(!time_buff){
	    time_buff = new(std_Local, 
		     	    sizeof(std_Time),
		     	    &(std_Time){std_Time_Type}
			);
	}

return std.Clock.getTime(
	    std.Clock.getPrimary(), 
	    std.Local.getData(time_buff)
       );
}

std_Time*  methodimpl(std_Clock, getTime, std_Time* time_buff){
	return this.system->getTime(time_buff);
}

static std_Clock* Primary_Clock = null;

errvt       methodimpl(std_Clock, setPrimary){ Primary_Clock = self; return OK; }
std_Clock* vmethodimpl(std_Clock, getPrimary){ return Primary_Clock; }

construct(std_Clock,
FMT(), 
DEF(), 
){ 
    nonull(arg.clockSystem, return nil);
    this.system = arg.clockSystem; 
return self; }

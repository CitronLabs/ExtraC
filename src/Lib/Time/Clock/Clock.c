#include <XC.pkg.c>

#define module std, Clock


std_Time* moduleFn(System_UTC_getTime)(std_Time* time_buff){
	XC_Time XCtime_buff;
	core.System.Time.getNow(core.System.Time.Source.REALTIME, &XCtime_buff);
	
	time_buff->seconds = XCtime_buff.seconds;
	time_buff->nanosec = XCtime_buff.nanoseconds;

return time_buff;
}

std_Clock* moduleFn(System_UTC_getClock)(){
	static std_Clock clock = {0};

	if(!clock.sys.getTime){
	    create(std_Clock, &clock, std.Clock.Systems.UTC.interface);
	}

return &clock;
}

std_Time* moduleFn(System_TAI_getTime)(std_Time* time_buff){
	XC_Time XCtime_buff;
	core.System.Time.getNow(core.System.Time.Source.MONOTONIC, &XCtime_buff);
	
	time_buff->seconds = XCtime_buff.seconds;
	time_buff->nanosec = XCtime_buff.nanoseconds;

return time_buff;
}

std_Clock* moduleFn(System_TAI_getClock)(){
	static std_Clock clock = {0};

	if(!clock.system){
	    create(std_Clock, &clock, std.Clock.Systems.TAI.interface);
	}

return &clock;
}

#define NUMBER_OF_MILLISEC_IN_MICROSEC (100 * 100 * 100)
#define TAI_TO_TT_INTEGER_OFFSET 32
#define TAI_TO_TT_DECIMAL_OFFSET (184 * NUMBER_OF_MILLISEC_IN_MICROSEC)

std_Time* moduleFn(System_TT_getTime)(std_Time* time_buff){
	XC_Time XCtime_buff;
	core.System.Time.getNow(core.System.Time.Source.MONOTONIC, &XCtime_buff);
	
	len_t carry = 
		(XCtime_buff.nanoseconds + TAI_TO_TT_DECIMAL_OFFSET) / 1000000000;
			
	time_buff->nanosec = (XCtime_buff.nanoseconds + TAI_TO_TT_DECIMAL_OFFSET) % 1000000000 ;
	time_buff->seconds = XCtime_buff.seconds +  TAI_TO_TT_INTEGER_OFFSET + carry;

return time_buff;
}

std_Clock* moduleFn(System_TT_getClock)(){
	static std_Clock clock = {0};

	if(clock.system.getTime){
	    create(std_Clock, &clock, std.Clock.Systems.TT.interface);
	}

return &clock;
}


std_Time*  moduleFn(getNow)(){
	thread_local static std_Time time_buff = {0};
	
return std.Clock.getTime(
	    std.Clock.getPrimary(), 
	    &time_buff
       );
}

std_Time*  moduleMethod(std_Clock, getTime, std_Time* time_buff){
	return this.system->getTime(time_buff);
}

static std_Clock* Primary_Clock = nil;

errvt      moduleMethod(std_Clock, setPrimary){ Primary_Clock = self; return OK; }
std_Clock* moduleFn(getPrimary)(){ return Primary_Clock; }

construct(std_Clock,
FMT(), 
DEF(), 
){ 
    nonull(arg.clockSystem){ return nil; }
    this.system = arg.clockSystem; 
return self; }

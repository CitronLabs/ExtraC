#pragma once

#define __XC_TIME__
#include "../pkg.h"
#include "../types.h"

#define package std

Decl(Time)
Decl(Clock)

Class(Watch,
INIT(
	std_Time* limit;
	std_Clock* clock;
	namespace(flags,
      	    bool 
	   	startOnInit  : 1,
	   	initTimeline : 1;
	)
),
FMT(),
	std_Time* start_time;
){
    errvt method(Watch, Start);
    std_Time* method(Watch, Check);
    std_Time* method(Watch, Stop);

    namespace(Timeline,
	errvt 		  method(Watch, Init);
	errvt 		  method(Watch, Merge,    Buffer(std_Watch) merge_buff);
	errvt 		  method(Watch, MergeOne, std_Watch* merge_watch);
	std_EventID 	  method(Watch, newEvent, std_Event* event);
	Buffer(std_Event) method(Watch, getEvents);
    )
};

#undef package

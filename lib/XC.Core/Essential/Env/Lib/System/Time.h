#pragma once
#include "../types.h"

typedef struct XC_Time {
    	u64 seconds;
    	u64 nanoseconds;
} XC_Time;


Interface(__XC_SYS_TIME,
	values(Source, word,
		REALTIME,
		MONOTONIC
	)
      	errvt fn(getNow)(word src, XC_Time* ts);
      
      	errvt fn(sleep)(const XC_Time* duration);
)

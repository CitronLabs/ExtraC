#pragma once
#include "../types.h"

typedef struct timeSpec {
    	u64 seconds;
    	u64 nanoseconds;
} timeSpec;


Interface(__XC_TIME,
	values(Source, word,
		REALTIME,
		MONOTONIC
	)
      	errvt fn(getNow)(word src, timeSpec* ts);
      
      	errvt fn(sleep)(const timeSpec* duration);
      
      	len_t fn(tickCount)(); 
      	len_t fn(tickFreq)(); 

)

#pragma once
#define __XC_OS_ENV_SYSTEM_SCHEDULER__
#include "pkg.h"

#define package os_Env_Scheduler

#define SchedulerHandle os_Env_Scheduler_Handle


typefrom(pntr, Handle);


#undef package
#define package os_Env_Scheduler_Process


typefrom(word, EventType);

type(Event,
	SchedulerHandle handle;
	os_Env_Scheduler_Process_EventType type;

     	union {
     		void* break_address;
     	} data;
)

#undef package


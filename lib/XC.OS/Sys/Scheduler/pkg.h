#pragma once
#define __XC_OS_SYSTEM__
#include "../pkg.h"


#ifndef __XC_OS_SYSTEM_SCHEDULER__

#include "types.h"

Interface(Scheduler,
	values(Version, word,
		MAJOR,
		MINOR,
		PATCH
	)

	errvt 		fn(initSystem);
	errvt 		fn(exitSystem);
	u64  	 	fn(pollEvents);

	submodule(Thread,
		noFail 		fn(sleep)(u64 millisec);
		SchedulerHandle fn(init)(void fn(thread_start)(void* args), void* args);
		errvt 		fn(exit)(SchedulerHandle handle);
		errvt 		fn(wait)(SchedulerHandle handle);
		SchedulerHandle fn(getCurrent);
		errvt 		fn(handleEvents)(SchedulerHandle handle, ArrayQueue(os_Event) evntQueue);
		u64  	 	fn(pollEvents);
	)

	submodule(Process,
	  values(Flags, word,
		DEBUG
	  )
	  values(Event, word,
		BREAKPOINT,
		EXCEPTION,
		CREATE_PROCESS,
		EXIT_PROCESS,
		LOAD_LIB,
		UNLOAD_LIB
	  )
		bool 		fn(isRunning)(SchedulerHandle handle);
		SchedulerHandle fn(init)	(strc8 exePath, strc8 args, u32 flags);
		errvt 		fn(read)	(SchedulerHandle handle, void* address, void* buffer, len_t size);
		errvt 		fn(write)	(SchedulerHandle handle, void* address, void* buffer, len_t size);
		errvt 		fn(cont)	(SchedulerHandle handle);
		errvt 		fn(detach)	(SchedulerHandle handle);
		errvt 		fn(handleEvents)(SchedulerHandle handle, ArrayQueue(os_Event) evntQueue);
		errvt 		fn(kill)	(SchedulerHandle handle);
		u64  	 	fn(pollEvents);
	)

	submodule(Mutex,
		SchedulerHandle fn(init);
		errvt 		fn(lock)	(SchedulerHandle handle);
		errvt 		fn(unlock)	(SchedulerHandle handle);
		errvt 		fn(tryLock)	(SchedulerHandle handle);
		errvt 		fn(destroy)	(SchedulerHandle handle);
	)

	submodule(Semaphore,
		SchedulerHandle fn(init)	(len_t num);
		errvt 		fn(wait)	(SchedulerHandle handle);
		errvt 		fn(post)	(SchedulerHandle handle);
		errvt 		fn(tryWait)	(SchedulerHandle handle);
		errvt 		fn(destroy)	(SchedulerHandle handle);
	)
)

#undef package
#undef SchedulerHandle
#endif


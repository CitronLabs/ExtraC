#pragma once
#include "../types.h"


Interface(__XC_SYS_THREAD,
	noFail 		fn(sleep)(len_t millisec);
	threadHandle 	fn(init)(errvt fn(thread_start)(void* args), void* args);
	errvt 		fn(run)(threadHandle thread);
	errvt 		fn(set)(threadHandle thread, errvt fn(thread_start)(void* args), void* args);
	errvt 		fn(exit)();
	errvt 		fn(free)(threadHandle thread);
	errvt 		fn(onInit)(bool fn(func)(void* args), void* args);
	errvt 		fn(onExit)(bool fn(func)(void* args), void* args);
	threadHandle 	fn(getCurrent)();
)


static inline void _auto_unlock(busylock **m) {if (*m) {_INTERNAL_RELEASE(*m);}}

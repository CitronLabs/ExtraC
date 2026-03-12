#pragma once
#define __XC_SCHEDULER__
#include "../pkg.h"

#ifndef __XC_SCHEDULER_THREAD__
#define package std

Class(Mutex,
INIT(),
FMT(),
){
      	errvt method(Mutex,Lock);
      	errvt method(Mutex,Unlock);
      	errvt method(Mutex,TryLock);
};


Class(Semaphore,
INIT(u64 slots;),
FMT(),
){
	errvt method(Semaphore,Wait);
	errvt method(Semaphore,Post);
	errvt method(Semaphore,TryWait);
};

Class(Thread,
INIT(errvt fn(func)(std_Thread*, void*)),
FMT(),

){
	interface(std_Mutex) 	 Mutex;
	interface(std_Semaphore) Semaphore;

	errvt 		method(Thread, Start, void* args);
	errvt 		method(Thread, Join);
	errvt 		method(Thread, Free);
	errvt 		method(Thread, Exit,int exitcode);
	errvt 		method(Thread, GetExitCode, int* result);
	void 	   	fn(Sleep)(u64 milliseconds);
	std_Thread* 	fn(GetCurrent)();

};

#undef package
#endif

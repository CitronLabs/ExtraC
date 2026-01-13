#pragma once
#define __XC_OS__
#include "../pkg.h"

#define package os
Class(Process,
INIT(),
FMT(),

){

};

Class(Thread,
INIT(errvt fn(func)(os_Thread*, void*)),
FMT(),

){
	errvt 		method(Thread, Start, void* args);
	errvt 		method(Thread, Join);
	errvt 		method(Thread, Free);
	errvt 		method(Thread, Exit,int exitcode);
	errvt 		method(Thread, GetExitCode, int* result);
	void 	   	fn(Sleep)(u64 milliseconds);
      	os_Thread* 	fn(GetCurrent)();

};

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

#undef package

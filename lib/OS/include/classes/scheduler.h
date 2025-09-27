#pragma once
#include "../OS.h"


Class(Process,
INIT(),
FIELD(),

)


Class(Thread,
INIT(int(*func)(inst(Thread), void* args);),
FIELD(),

	errvt method(Thread,Start,, void* args);
	errvt method(Thread,Join);
	errvt method(Thread,Free);
	errvt method(Thread,Exit,,int exitcode);
	errvt method(Thread,GetExitCode,, int* result);
      	inst(Error) method(Thread,GetErr);
	void (*Sleep)(u64 milliseconds);
      	inst(Thread)(*GetCurrent)();
);
Class(Mutex,
INIT(),
FIELD(),
      	errvt method(Mutex,Lock);
      	errvt method(Mutex,Unlock);
      	errvt method(Mutex,TryLock);
);
Class(Semaphore,
INIT(u64 slots;),
FIELD(),
      	errvt method(Semaphore,Wait);
      	errvt method(Semaphore,Post);
      	errvt method(Semaphore,TryWait);
);

#pragma once
#define __XC_SCHEDULER__
#include "../pkg.h"

#ifndef __XC_SCHEDULER_COROUTINE__
#define package std


#define go(name)
#define yield()
#define stop()

Class(Coroutine, 
INIT(),
FMT(),
){
	void 		fn(Yield)();
	void 		fn(Exit)();
	std_Coroutine* 	fn(This)();
	void 		fn(Join)(std_Coroutine* cr);
	void 		method(Coroutine, Start, void* args);

};

#undef package
#endif

#pragma once
#define __XC_FEATURE__
#include "../pkg.h"
#include "../Type/pkg.h"
#include "../Arrays/pkg.h"


//=======EXPORTS========
#ifndef __XC_PROCESS__
#define package std

#define Process(name, Class, ...) 				\
	const struct ___(Class, name, Proc) 			\
	    { noFail 						\
		fn(end)(), fn(fail)(), 				\
		fn(doRun)();					\
								\
		Class* fn(result)();				\
								\
		const struct ___(Class, name, Proc)		\
		fn(start)(Class*) 				\
		__VA_OPT__(,)					\
			__VA_ARGS__; } name;


#define doRun(times)		\
	doRun(); loop(i, times)	\


Class(Process,
INIT(Type_t class; void* extraData),
FMT(),
	void* extraData;
private(
	void* 			activeResult;
	ArrayStack(void*) 	activeResultStack;
)
){
	errvt method(Process, end);
	void* method(Process, result);
	errvt method(Process, start, void* data);

	noFail fn(noOp)();
};

#undef package
#endif

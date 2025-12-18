#pragma once
#define __XC_FEATURE__
#include "../pkg.h"


//=======EXPORTS========
#ifndef __XC_PROCESS__

#define Process(Class, ...) 					\
	const struct ___(package, Class##_Proc) 		\
	    { noFail 						\
		fn(end)(), fn(fail)(), 				\
		fn(doRun)(), fn(pause)();			\
								\
		___(package, Class) fn(current)();		\
								\
		const struct ___(package, Class##_Proc)		\
		fn(start)(___(package, Class)*), 		\
		fn(setProc)					\
			(struct ___(package, Class##_Proc)**), 	\
		fn(cont)()__VA_OPT__(,)				\
			__VA_ARGS__; } Process;














#endif

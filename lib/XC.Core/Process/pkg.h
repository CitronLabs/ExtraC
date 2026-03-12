#pragma once
#define __XC_CORE__
#include "../pkg.h"
#include "../Type/pkg.h"
#include "../Arrays/pkg.h"


//=======EXPORTS========
#ifndef __XC_PROCESS__
#define package std

#define Process(name, Class, ...) 				\
	const struct ___(Class, name, Proc) 			\
	    { noFail 						\
		fn(end)(), fn(fail)(); 				\
								\
		Class* fn(result)();				\
								\
		const struct ___(Class, name, Proc)		\
		fn(run)(bool cont),				\
		fn(start)(Class*) 				\
		__VA_OPT__(,)					\
			__VA_ARGS__; } name;

#define run_continue true
#define run_stop     false

#undef package
#endif

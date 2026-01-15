#pragma once
#define __XC_ENV__
#include "../pkg.h"
#include "../Lib/pkg.h"

#ifndef __XC_ENV_RUNTIME__

Interface(App,
	values(Info, strc8,
		NAME,
		DOMAIN
	)
	errvt fn(start)(__);
)

extern const App_Interface __MAIN_APP;

#define setApp(module) 							\
	const App_Interface __MAIN_APP = __PATH_CAT_NAME(module); 	\

#define main(...) 							\
	errvt moduleFn(App_start)(__);					\
	const App_Interface __MAIN_APP = 				\
		{.start = mod(App_start), .Info = {__VA_ARGS__}}; 	\
	errvt moduleFn(App_start)(__)
#endif


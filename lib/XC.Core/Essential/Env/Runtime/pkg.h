#pragma once
#define __XC_ENV__
#include "../pkg.h"
#include "../Lib/pkg.h"

#ifndef __XC_ENV_RUNTIME__

Interface(App,
	values(Info, strc8,
		NAME,
		DOMAIN,
		DESCRIPTION
	)
	errvt fn(start)(__);
)

Interface(Module,
	submodule(Info,
	    strc8
	   	NAME,
		DESCRIPTION;
	)
	errvt fn(start)(__);
	errvt fn(end)(__);
)

extern const App_Interface __MAIN_APP;

#define setApp(module) 							\
	const App_Interface __MAIN_APP = __PATH_CAT_NAME(module); 	\

#define main(...) 							\
	errvt moduleFn(App_start)(__);					\
	const App_Interface __MAIN_APP = 				\
		{.start = mod(App_start), .Info = {__VA_ARGS__}}; 	\
	errvt moduleFn(App_start)(__)


#define moduleExport(...) 			\
	const Module_Interface __MODULE = {	\
		.start = mod(start), 		\
		.end   = mod(end), 		\
		.Info  = {__VA_ARGS__}};
	

#define moduleStart() 						\
	errvt moduleFn(start)(__)

#define moduleEnd() 						\
	errvt moduleFn(start)(__)

#endif


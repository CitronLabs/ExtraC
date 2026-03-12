#pragma once
#include "../types.h"
#include "../utils.h"

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





typedef const struct App_Interface {
	struct {
	    const strc8
		NAME,
		DOMAIN,
		DESCRIPTION;
	} Info;
	errvt (* const start)(__);
} App_Interface;

typedef const struct Module_Interface {
	struct {
	    const strc8
		NAME,
		DOMAIN,
		DESCRIPTION;
	} Info;
	errvt (* const start)(__);
	errvt (* const end)(__);
} Module_Interface;

extern const App_Interface __MAIN_APP;


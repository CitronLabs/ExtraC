#pragma once
#include "../libc.h"
#include "basetypes.h"


#define InType(name, type)  typedef struct {const _Atomic(bool)* isReady; type* data;}in_##name;
#define OutType(name, type) typedef type out_##name;

#define SETUP(...) __VA_ARGS__;
#define IO(...) __VA_ARGS__;
#define PARAM(...) __VA_ARGS__;


#define Logic(name) void name##_Logic(typeof(name)* self, name##_Params* p)
#define Setup(name) errvt name##_Setup(typeof(name)* self, name##_SetUpArgs* args)

#define Module(name, SETUP, IO, PARAM, ...) 					\
	typedef struct {_Atomic(bool) ready; IO} name##_Ports;			\
	typedef struct {name##_Ports ports; __VA_ARGS__} name##_Module; 	\
	static name##_Module name;						\
	typedef struct {PARAM} name##_Params;					\
	typedef struct {SETUP} name##_SetUpArgs; 				\
	Logic(name); Setup(name)


#define setports(module) module.ports = (typeof(module.ports))
#define setup(module, ...) module##_Setup(&module, &(module##_SetUpArgs){__VA_ARGS__})
#define run(module, ...) 						\
	void module##_Logic(typeof(module)*, module##_Params*); 		\
	if(module.ports.ready){ 					\
	  module.ports.ready = false;					\
	  module##_Logic(&module, &(module##_Params){__VA_ARGS__});	\
	  module.ports.ready = true;					\
	}

#define out(outName) self->ports.outName 
#define ready(inName) (*self->ports.inName.isReady) 
#define in(var, inName) while (true) if(ready(inName)){var = *self->ports.inName.data; break;} 

#define modl(module) module##_Module*

InType(var, void);



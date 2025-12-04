#pragma once
#define __XC_ESSENTIAL__
#include "pkg.h"

#include "macro_utils.h"

#define interfaceOf(name) name##_Interface

#define Interface(name, ...) 			\
	typedef struct interfaceOf(name)	\
	{__VA_ARGS__} interfaceOf(name); 	\


#define Module(name) typedef struct name##_Interface name##_Interface; struct name##_Interface

#define import(interface) extern const interface##_Interface interface;

#define importFn(...) \
	typedef typeof(__PATH_CAT_NAME(module)) __MODULE_CAT_NAME(module, Interface); \
	__FUNCS_DECL(__MODULE_CAT_NAME(module), __VA_ARGS__)

#define export(...) 								\
 __MODULE_DEFINE(__MODULE_CAT_NAME(module), __VA_ARGS__)

#define Impl(name) 	    const interfaceOf(name) name = 				
#define ImplAs(Class, name) interfaceOf(Class) name = 				


#define submodule(name, ...) const struct {__VA_ARGS__} name;
#define alias(name, aliasName) typeof(name) const* aliasName = &name;

#define values(name, type, ...) struct { const type __VA_ARGS__; } name;


#define method(Class,name, ...) (*name)(___(package,Class)* self __VA_OPT__(, __VA_ARGS__))
#define imethod(name, ...) 	(*name)(void* object __VA_OPT__(, __VA_ARGS__))
#define vmethod(name, ...) 	(*name)(__VA_ARGS__)
#define fn(name) (*const name) 
#define localFn(name) static name
#define moduleFn(name) ___(__MODULE_CAT_NAME(module), name)


#define methodimpl(Class,Routine, ...) 			\
	Class##_##Routine(Class* self __VA_OPT__(, __VA_ARGS__))

#define imethodimpl(Class,Routine, ...) 		\
	Class##_##Routine(void* object __VA_OPT__(, __VA_ARGS__))

#define vmethodimpl(Class,Routine, ...) 		\
	Class##_##Routine(__VA_ARGS__)

#define interface(Class) interfaceOf(Class) 


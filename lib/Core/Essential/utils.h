#pragma once
#define __XC_ESSENTIAL__
#include "pkg.h"

#include "macro_utils.h"

#define Interface(name, ...) 			\
	typedef const struct name##_Interface	\
	{__VA_ARGS__} name##_Interface; 	\


#define Module(name) typedef struct name##_Interface name##_Interface; struct name##_Interface

#define import(interface) extern const interface##_Interface interface;

#define importFn(...) \
	__FUNCS_DECL(__MODULE_CAT_NAME(module), __VA_ARGS__);

#define export(...) 								\
 const __MODULE_DEFINE(__MODULE_CAT_NAME(module), __VA_ARGS__)

#define Impl(name) 	    		const name##_Interface name = 				
#define ImplAs(interface, name) 	const interface##_Interface name = 				


#define submodule(name, ...) const struct {__VA_ARGS__} name;
#define alias(name, aliasName) typeof(name) const* aliasName = &name;

#define values(name, type, ...) struct { const type __VA_ARGS__; } name;


#define method(Class,name, ...) (*name)(___(package,Class)* self __VA_OPT__(, __VA_ARGS__))
#define imethod(name, ...) 	(*name)(void* object __VA_OPT__(, __VA_ARGS__))

#define fn(name)  (*const name) 
#define vfn(name) (*name)


#define moduleFn(name) ___(__MODULE_CAT_NAME(module), name)
#define moduleMethod(Class, name, ...) ___(__MODULE_CAT_NAME(module), name)(Class* self __VA_OPT__(, __VA_ARGS__))
#define moduleIMethod(Class, name, ...) ___(__MODULE_CAT_NAME(module), name)(void* object __VA_OPT__(, __VA_ARGS__))
#define moduleValues(name, ...)	const typeof(__PATH_CAT_NAME(module, name)) __MODULE_CAT_NAME(module, name) = {__VA_ARGS__} \

#define interface(name) const name##_Interface 


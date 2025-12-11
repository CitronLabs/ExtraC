#pragma once
#define __XC_ESSENTIAL__
#include "pkg.h"

#include "__macro_utils.h"

#define Interface(name, ...) 			\
	typedef const struct name##_Interface	\
	{__VA_ARGS__} name##_Interface; 	\


#define Module(name) 									\
	typedef struct name##_Interface name##_Interface; struct name##_Interface	\

#define importFn(...) \
	__FUNCS_DECL(__MODULE_CAT_NAME(module), __VA_ARGS__);

#define SUBMODULE(...) __VA_ARGS__
#define VALUES(...) __VA_ARGS__

#define export(SUBMODULE, VALUES, ...) 							\
__VA_OPT__(importFn(__VA_ARGS__))							\
static const typeof(__PATH_CAT_NAME(((PP_PASTE(__GET_FIRST(module), _Interface)){0}),   \
                __GET_REST(module))) PP_PASTE(__MODULE_CAT_NAME(module), _Module) = {   \
    	SUBMODULE_DEFINE(__MODULE_CAT_NAME(module), SUBMODULE) 				\
	VALUES_DEFINE(__MODULE_CAT_NAME(module), VALUES)				\
          __VA_OPT__(PP_PASTE(                                                 		\
              __MODULE_DEFINE_INIT_,                                           		\
              GET_MEMBER_COUNT(                                                		\
                  0, __VA_ARGS__, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 		\
                  89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75,  		\
                  74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60,  		\
                  59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45,  		\
                  44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30,  		\
                  29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15,  		\
                  14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2,                  		\
                  1))(__MODULE_CAT_NAME(module), __VA_ARGS__))};

#define exportModule(SUBMODULE, VALUES, ...) 						\
__VA_OPT__(importFn(__VA_ARGS__))							\
static const PP_PASTE(__MODULE_CAT_NAME(module), _Interface) __MODULE_CAT_NAME(module)={\
    	SUBMODULE_DEFINE(__MODULE_CAT_NAME(module), SUBMODULE) 				\
	VALUES_DEFINE(__MODULE_CAT_NAME(module), VALUES)				\
        __VA_OPT__(PP_PASTE(__MODULE_DEFINE_INIT_, 					\
            GET_MEMBER_COUNT(0, __VA_ARGS__, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))		\
		   (__MODULE_CAT_NAME(module), __VA_ARGS__)) }; 				\

#define Impl(name) 	    		const name##_Interface name = 				
#define ImplAs(interface, name) 	const interface##_Interface name = 				


#define submodule(name, ...) const struct {__VA_ARGS__} name;
#define alias(path, alias) typeof(path) alias = path;

#define values(name, type, ...) struct { const type __VA_ARGS__; } name;


#define method(Class,name, ...) (*name)(___(package,Class)* self __VA_OPT__(, __VA_ARGS__))
#define imethod(name, ...) 	(*name)(void* object __VA_OPT__(, __VA_ARGS__))

#define fn(name)  (*const name) 
#define vfn(name) (*name)


#define moduleFn(name) ___(__MODULE_CAT_NAME(module), name)
#define moduleMethod(Class, name, ...) ___(__MODULE_CAT_NAME(module), name)(Class* self __VA_OPT__(, __VA_ARGS__))
#define moduleIMethod(Class, name, ...) ___(__MODULE_CAT_NAME(module), name)(void* object __VA_OPT__(, __VA_ARGS__))


#define moduleValues(name, ...)											\
    enum { 													\
	__VALUES_ENUM_FOR_EACH_DISPATCH(__VALUES_ENUM_IMPL, __MODULE_CAT_NAME(module, name), __VA_ARGS__) }; 	\
   typeof(__GET_MEMBER_PATH(name)) __MODULE_CAT_NAME(module, name) = {	\
	__VALUES_DEFINE_FOR_EACH_DISPATCH(__VALUES_DEFINE_IMPL, __MODULE_CAT_NAME(module, name), __VA_ARGS__)};	

#define interface(name) const name##_Interface 


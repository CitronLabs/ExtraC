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

#define importFnFrom(PATH, ...) 	\
	__SUBFUNCS_DECL(__MODULE_CAT_NAME(module, PATH), __PATH_CAT_NAME(PATH), __VA_ARGS__);

#define SUBMODULE(...) __VA_ARGS__
#define VALUES(...) __VA_ARGS__
#define PATH(...) __VA_ARGS__
#define __PATH(...) __VA_ARGS__

#define export(SUBMODULE, VALUES, ...) 							\
__VA_OPT__(importFn(__VA_ARGS__))							\
static const typeof(__PATH_CAT_NAME(((PP_PASTE(__GET_FIRST(module), _Interface)){})   	\
                __GET_REST_PATH_VERIFY(module))) __MODULE_CAT_NAME_VERIFY(module) = {   \
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

#define exportFrom(PATH, SUBMODULE, VALUES, ...) 					\
__VA_OPT__(importFnFrom(__PATH(PATH), __VA_ARGS__))					\
static const typeof(__PATH_CAT_NAME(((PP_PASTE(__GET_FIRST(module, PATH),_Interface)){})\
                __GET_REST_PATH_VERIFY(module, PATH))) 					\
		__MODULE_CAT_NAME_VERIFY(module, PATH) = {   				\
    	SUBMODULE_DEFINE(__MODULE_CAT_NAME(module, PATH), SUBMODULE) 			\
	VALUES_DEFINE(__MODULE_CAT_NAME(module, PATH), VALUES)				\
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
                  1))(__MODULE_CAT_NAME(module, PATH), __VA_ARGS__))};


#define exportDef(...) 									\
static const typeof(__PATH_CAT_NAME(((PP_PASTE(__GET_FIRST(module), _Interface)){})   	\
        __GET_REST_PATH_VERIFY(module))) __MODULE_CAT_NAME_VERIFY(module)={__VA_ARGS__};\


#define Impl(name) 	    		const name##_Interface name = 				
#define ImplAs(interface, name) 	const interface##_Interface name = 				


#define submodule(name, ...) struct {__VA_ARGS__} name;
#define alias(path, alias) const typeof(path) alias = path;

#define values(name, type, ...) struct { const type __VA_ARGS__; } name;


#define method(Class,name, ...) (*name)(___(package,Class)* self __VA_OPT__(, __VA_ARGS__))
#define imethod(name, ...) 	(*name)(void* object __VA_OPT__(, __VA_ARGS__))

#define fn(name)  (*const name) 
#define vfn(name) (*name)


#define moduleFn(name) ___(__MODULE_CAT_NAME(module), name)
#define moduleMethod(Class, name, ...) ___(__MODULE_CAT_NAME(module), name)(Class* self __VA_OPT__(, __VA_ARGS__))
#define moduleIMethod(name, ...) ___(__MODULE_CAT_NAME(module), name)(void* object __VA_OPT__(, __VA_ARGS__))

#define moduleValue(name) static const typeof(__GET_MEMBER_PATH(name)) __MODULE_CAT_NAME(module, name)

#define moduleValues(name, ...)											\
    enum { 													\
	__VALUES_ENUM_FOR_EACH_DISPATCH(__VALUES_ENUM_IMPL, __MODULE_CAT_NAME(module, name), __VA_ARGS__) }; 	\
   static const typeof(__GET_MEMBER_PATH(name)) __MODULE_CAT_NAME(module, name) = {				\
	__VALUES_DEFINE_FOR_EACH_DISPATCH(__VALUES_DEFINE_IMPL, __MODULE_CAT_NAME(module, name), __VA_ARGS__)};	


#define moduleValuesDef(name, ...) 			\
	static const typeof(__GET_MEMBER_PATH(name))	\
	__MODULE_CAT_NAME(module, name) 		\
		= { __VA_ARGS__ };

#define switchV(value) 						\
	  for(int __num_iter = 0; __num_iter < 1;)		\
	    for(typeof(value) __switch_val = value; 		\
	    __num_iter < 1 ; __num_iter = 5)

#define caseV(value) if(__switch_val == value)

#define defaultV for(typeof(__switch_val) val = __switch_val; __num_iter; __num_iter = 5)

#define interface(name) const PP_PASTE(name, _Interface)
#define vtable(name) PP_PASTE(name, _VTable_Interface)

#define STRINGIFY_IMPL(...) #__VA_ARGS__
#define STRINGIFY(...) STRINGIFY_IMPL(__VA_ARGS__)

#define __module__ STRINGIFY(__PATH_CAT_NAME(module))

#pragma once
#define __XC_TYPES__
#include "../pkg.h"
#include "../types.h"

#define package std

#include "macro_utils.h"

type_fn(void*, Types_Alloc_Constructor, void*, void*);
type_fn(errvt, Types_Alloc_Free, void*);

Interface(Alloc_Ops,
	void* fn(init)(std_typeData* type, bool freeOnFail, std_Object* self, void* args);
	void* fn(initWith)(std_Types_Alloc_Constructor constructor, bool freeOnFail, std_Object* self, void* args);
	errvt fn(destruct)(bool doFree, void* delObjs[], len_t delObjsNum);
	errvt fn(destructWith)(std_Types_Alloc_Free freeFunc, void* delObjs[], len_t delObjsNum);
)

#define new_alloc(name) malloc(sizeof(name))
		
#define push_alloc(name) alloca(sizeof(name))

#define create(name, ptr, ...) 					\
	(name*) std.Types.alloc.init(				\
		generic name##_Type,  		    		\
		false, 						\
		generic ptr, 					\
		__CONSTRUCT_ARGS(name 				\
		  __VA_OPT__(, __VA_ARGS__))			\
	)



#define create_with(name, constructor, ptr, ...) 				\
	(name*) std.Types.alloc.initWith(					\
  		generic name##_CONSTRUCTOR_##constructor##_Type->ops->Create, 	\
		false, 								\
		generic ptr, 							\
		__CONSTRUCT_ARGS(						\
		   name##_CONSTRUCTOR_##constructor 				\
		  __VA_OPT__(, __VA_ARGS__))					\
	)


#define new(name, ...)  (name*)std.Types.alloc.init(generic name##_Type, true, new_alloc(name), __CONSTRUCT_ARGS(name __VA_OPT__(, __VA_ARGS__)))
#define push(name, ...) (name*)std.Types.alloc.init(generic name##_Type, false, push_alloc(name), __CONSTRUCT_ARGS(name __VA_OPT__(, __VA_ARGS__)))


#define new_with(name, constructor, ...)  					\
	(name*)std.Types.alloc.initWith(					\
  		generic name##_CONSTRUCTOR_##constructor##_Type->ops->Create, 	\
		true, new_alloc(name), 						\
		__CONSTRUCT_ARGS(						\
		   name##_CONSTRUCTOR_##constructor 				\
		   __VA_OPT__(, __VA_ARGS__))					\
	)
#define push_with(name, constructor, ...) 					\
	(name*)std.Types.alloc.initWith(					\
  		generic name##_CONSTRUCTOR_##constructor##_Type->ops->Create, 	\
		false, push_alloc(name), 					\
		__CONSTRUCT_ARGS(						\
		   name##_CONSTRUCTOR_##constructor 				\
		   __VA_OPT__(, __VA_ARGS__))					\
	)

#define del(...) std.Types.alloc.destruct(true, (pntr[]){__VA_ARGS__}, sizeof((pntr[]){__VA_ARGS__}) / sizeof(pntr))
#define pop(...) std.Types.alloc.destruct(false, (pntr[]){__VA_ARGS__}, sizeof((pntr[]){__VA_ARGS__}) / sizeof(pntr))
#define destroy(...) std.Types.alloc.destruct(false, (pntr[]){__VA_ARGS__}, sizeof((pntr[]){__VA_ARGS__}) / sizeof(pntr))

#define DESTROY(class) errvt moduleMethod(class,  Op_Destroy)							
#define CREATE(class)  class* moduleMethod(class, Op_Create, len_t size, __CONCATE_IMPL(class,ConstructArgs)* args)


#define __SCOPE_IMPL(var, ...)  for(std_Object* __var = generic (var); __var; (__var->__type->ops->Destroy(__var),__var = nil)) \
				__VA_OPT__(for(typeof(__var) __VA_ARGS__ = __var; __VA_ARGS__; __VA_ARGS__ = nil))

#define scope(var, ...) __SCOPE_IMPL(var __VA_ARGS__)

#define construct_with(name, constructor, DEF)  			\
name* moduleMethod(name, ___(CONSTRUCTOR, constructor, Op_Create), 	\
len_t size, ___(name, CONSTRUCTOR, constructor, ConstructArgs) *args);	\
	const ___(name, CONSTRUCTOR, constructor, ConstructArgs)	\
		___(name, CONSTRUCTOR, constructor, DefaultArgs)	\
		= {DEF};						\
	const ___(name, CONSTRUCTOR, constructor, Ops_t) 		\
		___(name, CONSTRUCTOR, constructor, OpsImpl)		\
		= {.Create = mod(					\
		    ___(CONSTRUCTOR, constructor, Op_Create))};		\
	const ___(name, CONSTRUCTOR, constructor##_Type_t)		\
		___(name, CONSTRUCTOR, constructor, TypeData) = {	\
		.id        = generic name##_TypeID, 			\
		.size      = sizeof(name), 				\
		.format    = generic &name##_DefaultFormat,		\
		.ops       = generic &___(name, CONSTRUCTOR, constructor, OpsImpl),	\
		.construct = generic &___(name, CONSTRUCTOR, constructor, DefaultArgs),	\
	};									 	\
name* moduleMethod(name, ___(CONSTRUCTOR, constructor, Op_Create), 		 	\
len_t size, ___(name, CONSTRUCTOR, constructor, ConstructArgs) *args)



#define construct(name, FMT, DEF, ...)  			\
	CREATE(name); 						\
	const char name##_TypeID[] = #name;			\
	const name##_FormatArgs name##_DefaultFormat 		\
		= {FMT};					\
	const name##_ConstructArgs name##_DefaultArgs  		\
		= {DEF};					\
	const name##_Ops_t name##_OpsImpl 			\
		= {__VA_ARGS__};				\
	const name##_Type_t name##_TypeData = {			\
		.id        = generic name##_TypeID, 		\
		.size      = sizeof(name), 			\
		.ops       = generic &name##_OpsImpl,		\
		.format    = generic &name##_DefaultFormat,	\
		.construct = generic &name##_DefaultArgs,	\
	};							\
	CREATE(name)

#undef package




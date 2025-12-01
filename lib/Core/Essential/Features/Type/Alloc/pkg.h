#pragma once
#define __XC_TYPES__
#include "../pkg.h"

#include "../types.h"

#include "macro_utils.h"

Interface(Alloc_Ops,
	void* fn(init)(std_typeData* type, bool freeOnFail, std_Object* self, void* args);
	errvt fn(destroy)(bool doFree, void* delObjs[], len_t delObjsNum);
)

#define new_alloc(name) malloc(sizeof(name))
		
#define push_alloc(name) alloca(sizeof(name))

#define create(name, ptr, ...) 				\
	(name*) std.Type.alloc.init(			\
		generic name##_Type,  		    	\
		false, 					\
		generic ptr, 				\
		__CONSTRUCT_ARGS(name 			\
		  __VA_OPT__(, __VA_ARGS__))		\
	)


#define new(name, ...)  (name*)std.Type.alloc.init(generic name##_Type, true, new_alloc(name), __CONSTRUCT_ARGS(name __VA_OPT__(, __VA_ARGS__)))
#define push(name, ...) (name*)std.Type.alloc.init(generic name##_Type, false, push_alloc(name), __CONSTRUCT_ARGS(name __VA_OPT__(, __VA_ARGS__)))

#define del(...) std.Type.alloc.destroy(true, (pntr[]){__VA_ARGS__}, sizeof((pntr[]){__VA_ARGS__}) / sizeof(pntr))
#define pop(...) std.Type.alloc.destroy(false, (pntr[]){__VA_ARGS__}, sizeof((pntr[]){__VA_ARGS__}) / sizeof(pntr))

#define DESTROY(class) errvt methodimpl(class,  Op_Destroy)							
#define CREATE(class)  class* methodimpl(class, Op_Create, len_t size, __CONCATE_IMPL(class,ConstructArgs)* args)

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








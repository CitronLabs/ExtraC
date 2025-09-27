#pragma once
#include "../libc.h"
#include "basetypes.h"
#include "modules.h"

/*======================|
 *	Methods
======================*/

typedef struct Object_Instance Object_Instance;

static inline errvt __XC_Object_Destroy(bool doFree, void* objs[], size_t objsNum);
static inline errvt __XC_Object_Append(Object_Instance* obj, void* args[]);
static inline errvt __XC_Object_Set(void* pairs[], size_t pairsNum);
static inline errvt __XC_Object_Copy(void* pairs[], size_t pairsNum);

#define method(Class,name, ...) (*name)(Class##_Instance* self __VA_ARGS__)
#define imethod(name, ...) 	(*name)(inst(Object) object __VA_ARGS__)
#define vmethod(name, ...) 	(*name)(__VA_ARGS__);

#define methodimpl(Class,Routine, ...) 			\
	Class##_##Routine(Class##_Instance* self __VA_ARGS__)

#define imethodimpl(Class,Routine, ...) 		\
	Class##_##Routine(inst(Object) object __VA_ARGS__)

#define vmethodimpl(Class,Routine, ...) 		\
	Class##_##Routine(__VA_ARGS__)

#define interface(Class) Class##_Interface Class
#define interfaceAs(Class) Class##_Interface 


/*======================|
 *   Construct Utils
======================*/


#define construct(name, ...)						\
	const u64 sizeof_##name##_Private = sizeof(name##_Private);	\
	Impl(name){__VA_ARGS__}; 					\
	name##_Instance* name##__Create__(				\
		name##_ConstructArgs args, 				\
		name##_Instance* self);					\
	name##_Instance* name##_Construct(				\
		name##_ConstructArgs args, 				\
		name##_Instance* self){ 				\
	self->__methods = &name;					\
	self->__private = 						\
	(name##_Private*)&(((u8*)self)[sizeof(name##_Instance)]); 	\
		if(name##__Create__(args, self) == NULL){ 		\
		    ERR(ERR_INITFAIL, "failed to initialize " #name);	\
		    return self;}					\
	self->__init = true;						\
return self;}								\
									\
	name##_Instance* name##__Create__(				\
		name##_ConstructArgs args, 				\
		name##_Instance* self)					\

#define create(name)							\
	name##_Instance* name##_Construct(				\
		name##_ConstructArgs args, 				\
		name##_Instance* self) 					\


#define private(name, ...) 				  \
typedef struct name##_Private{__VA_ARGS__}name##_Private; \

#define setpriv(Class) *self->__private = (Class##_Private)

#define passover *self = args; return self;


/*======================|
 *   	 Types
======================*/

#define __TYPE_OPERATORS__
#include "../config.h"
#undef __TYPE_OPERATORS__


#define Ops(type) const TypeOperators type##_Ops

#define asClass(type)							\
	typedef type type##_Instance; 					\
	typedef type type##_ConstructArgs;				\
	InType(type, type); OutType(type, type);			\
	OPERATOR_FUNCS(type);						\
	extern Ops(type);						\
	static type##_Instance* type##_Construct(			\
		type##_ConstructArgs args,				\
		type##_Instance* self)					

#define asClassExt(type, construct_args)				\
	static const u64 sizeof_##type##_Private = 0;			\
	typedef type type##_Instance; 					\
	typedef struct {construct_args;} type##_ConstructArgs;		\
	InType(type, type); OutType(type, type);			\
	OPERATOR_FUNCS(type);						\
	extern Ops(type);						\
	static inline type##_Instance* type##_Construct(		\
		type##_ConstructArgs args,				\
		type##_Instance* self)					


#define Interface(name, ...) 						\
	typedef struct name##_Interface					\
	{__VA_ARGS__} name##_Interface; 				\
	InType(name##_Interface, intf(name)); 				\
	OutType(name##_Interface, intf(name))


#define VTable(name, ...)						\
	Interface(name, __VA_ARGS__)					\
	extern name##_Interface name;					\

#define Impl(name) 	    name##_Interface name = 				
#define ImplAs(Class, name) Class##_Interface name = 				

#define Decl(name) 							\
	typedef struct name##_Instance name##_Instance; 		\
	typedef struct name##_ConstructArgs name##_ConstructArgs;	\
	typedef struct name##_Interface	name##_Interface;		\
	OPERATOR_FUNCS(name);						\
	name##_Instance* name##_Construct(				\
		name##_ConstructArgs args,				\
		name##_Instance* self);					\

#define Class(name,INIT, FIELD, ...) 					\
	typedef struct name##_Private name##_Private;			\
	extern const u64 sizeof_##name##_Private;			\
	typedef struct name##_Instance name##_Instance; 		\
	typedef struct name##_ConstructArgs				\
	{INIT} name##_ConstructArgs;					\
	Interface(name, __VA_ARGS__) 					\
	extern name##_Interface name;					\
	OPERATOR_FUNCS(name);						\
	extern Ops(name);						\
	typedef struct name##_Instance{					\
		name##_Private* __private;				\
		TypeOperators* __ops;					\
		FIELD							\
		u8 __init : 1;						\
	}name##_Instance;						\
	name##_Instance* name##_Construct(				\
		name##_ConstructArgs args,				\
		name##_Instance* self);					\
	InType(name, inst(name)); OutType(name, inst(name))

#define Data(name, INIT,...) 						\
	typedef struct name name; 					\
	typedef struct name{TypeOperators* __ops; __VA_ARGS__}name;	\
	asClassExt(name, INIT)					


/*======================|
 *   	 Utils
======================*/

#define INIT(...) __VA_ARGS__;
#define FIELD(...) __VA_ARGS__;

#define isinit(object) (object->__init)


#define priv self->__private
#define self(Class) inst(Class) self = class(Class) object;

typedef void* inst;
#define inst(Class) Class##_Instance*
#define intf(Class) const Class##_Interface*
#define data(Class) Class##_Instance
#define ifob(Class) ifob

#define generic (void*)
#define class(type) (inst(type))
#define like(var) (typeof(var))

#define useIntf(obj, ...) (ifob){generic &((*obj->__methods)__VA_ARGS__), generic obj}






Decl(Object);


#undef OPERATOR
#define OPERATOR(class, name, returnval, ...) returnval (*name)(void* __VA_ARGS__)

struct(TypeOperators, OPERATOR_FUNCS(Object)) 

#undef OPERATOR
#define OPERATOR(class, name, returnval, ...) returnval class##__##name(void* object __VA_ARGS__)

struct(typeData,
     	TypeOperators* ops;
	size_t size;
)
struct(variableData,
	typeData type;
       	void* data;
)

#define typebasic(type)   (typeData){NULL, sizeof(var)}
#define type(class) 	  (typeData){(void*)&class##_Ops, class##__Size(NULL, false)}
#define var(class, data)  (variableData){type(class), &data}	
#define varbasic(data)	  (variableData){(typeData){NULL, sizeof(data)}, &data}	


struct(ifob, void* intf; inst(Object) obj;);




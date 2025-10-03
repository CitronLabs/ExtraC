#pragma once
#define __XC_TYPE__
#include "../pkg.h"
#include "../types.h"

/*======================|
 *	Methods
======================*/

typedef struct Object_Instance Object_Instance;

static inline errvt __XC_Object_Destroy(bool doFree, void* objs[], size_t objsNum);
static inline errvt __XC_Object_Append(Object_Instance* obj, void* args[]);
static inline errvt __XC_Object_Set(void* pairs[], size_t pairsNum);
static inline errvt __XC_Object_Copy(void* pairs[], size_t pairsNum);

#define method(Class,name, ...) (*name)(Class##_Instance* self __VA_ARGS__)
#define imethod(name, ...) 	(*name)(void* object __VA_ARGS__)
#define vmethod(name, ...) 	(*name)(__VA_ARGS__);
#define fn(name) (*const name) 

#define methodimpl(Class,Routine, ...) 			\
	Class##_##Routine(Class##_Instance* self __VA_ARGS__)

#define imethodimpl(Class,Routine, ...) 		\
	Class##_##Routine(void* object __VA_ARGS__)

#define vmethodimpl(Class,Routine, ...) 		\
	Class##_##Routine(__VA_ARGS__)

#define interface(Class) Class##_Interface Class
#define interfaceAs(Class) Class##_Interface 


/*======================|
 *   Construct Utils
======================*/

#define private(name, ...) 				  	\
typedef struct name##_Private{__VA_ARGS__}name##_Private; 	\

#define setpriv(Class) *self->__private = (Class##_Private)

#define passover *self = args; return self;


/*======================|
 *   	 Types
======================*/

#define Ops(type) const TypeOperators type##_Ops


#define asClass(type, INIT, ...)					\
	static const bool type##_hasPrivate = false;			\
	typedef type type##_Instance; 					\
	typedef struct {INIT} type##_ConstructArgs;			\
	OPERATOR_FUNCS(type);						\
	extern Ops(type);						\
	type##_Instance* type##_CREATE(					\
		type##_Instance* self, 					\
		size_t size, 						\
		type##_ConstructArgs* args				\
	)


#define Interface(name, ...) 						\
	typedef struct name##_Interface					\
	{__VA_ARGS__} name##_Interface; 				\


#define VTable(name, ...)						\
	Interface(name, __VA_ARGS__)					\
	extern name##_Interface name;					\

#define Impl(name) 	    name##_Interface name = 				
#define ImplAs(Class, name) Class##_Interface name = 				



#define Decl(name) 							\
	typedef struct name##_Instance name##_Instance; 		\
	typedef struct name##_ConstructArgs name##_ConstructArgs;	\
	typedef struct name##_Interface	name##_Interface;		\
	typedef struct in_##name in_##name;				\
	typedef name##_Instance out_##name;				\
	OPERATOR_FUNCS(name);						\
	extern Ops(type);						\

#define Class(name,INIT, FIELD, ...) 					\
	typedef struct name##_Private name##_Private;			\
	static const bool name##_hasPrivate = true;			\
	typedef struct name##_Instance name##_Instance; 		\
	typedef struct name##_ConstructArgs				\
	{INIT;} name##_ConstructArgs;					\
	Interface(name, __VA_ARGS__;) 					\
	extern name##_Interface name;					\
	OPERATOR_FUNCS(name);						\
	extern Ops(name);						\
	typedef struct name##_Instance{					\
		TypeOperators* __ops;					\
		name##_Private* __private;				\
		FIELD;							\
	}name##_Instance;						\
	name##_Instance* name##_CREATE(					\
		name##_Instance* self, 					\
		size_t size, 						\
		name##_ConstructArgs* args				\
	)

#define Data(name, INIT,...) 						\
	typedef struct name name; 					\
	typedef struct name{ 						\
		TypeOperators* __ops; __VA_ARGS__}name;			\
	asClass(name, INIT)					


/*======================|
 *   	 Utils
======================*/

Decl(Object);
Class(Object,,,){return self;}

#define typebasic(type)   (typeData){NULL, sizeof(var)}
#define type(class) 	  (typeData){(void*)&class##_Ops, class##__Size(NULL, false, NULL)}
#define var(class, data)  (variableData){type(class), &data}	
#define varbasic(data)	  (variableData){(typeData){NULL, sizeof(data)}, &data}	


struct(ifob, void* intf; inst(Object) obj;);




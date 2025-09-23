#pragma once
#include "./libc.h"
#include "./utils.h"

/*------------------------------|
       Modules Preprocessors	|
-------------------------------*/
#define InType(name, type)  typedef struct {const _Atomic(bool)* isReady; type* data;}in_##name;
#define OutType(name, type) typedef type out_##name;

#define __SETUP(...) __VA_ARGS__;
#define __IO(...) __VA_ARGS__;
#define __PARAM(...) __VA_ARGS__;
#define __DATA(...) __VA_ARGS__;

#define Blueprint(name, __IO, __DATA)						\
	typedef struct {_Atomic(bool) ready; __IO} name##_Ports;		\
	typedef struct {name##_Ports ports; __DATA} name##_Module; 		\

#define Logic(name) void name##_Logic(typeof(name)* self, name##_Params* p)
#define Setup(name) errvt name##_Setup(typeof(name)* self, name##_SetUpArgs* args)

#define Module(name, blueprint, __SETUP, __PARAM, ...) 	\
	blueprint##_Module name = {__VA_ARGS__};	\
	typedef struct {__PARAM} name##_Params;		\
	typedef struct {__SETUP} name##_SetUpArgs; 	\
	Setup(name); Logic(name);


#define register(...) static struct {__VA_ARGS__;}
#define setports(module) module.ports = (typeof(module.ports))
#define setup(module, ...) module##_Setup(&module, &(module##_SetUpArgs){__VA_ARGS__})
#define run(module, ...) 						\
	void module##_Logic(typeof(module)*, module##_Params*); 		\
	if(module.ports.ready){ 					\
	  module.ports.ready = false;					\
	  module##_Logic(&module, &(module##_Params){__VA_ARGS__});	\
	  module.ports.ready = true;					\
	}
#define inAny(inName)(*self->ports.inName.isReady) ? self->ports.inName.data
#define out(outName) self->ports.outName 
#define in(inName)  (*self->ports.inName.isReady) ? *self->ports.inName.data  
#define ready(inName) (*self->ports.inName.isReady) 
#define set(var, inName) if(ready(inName)){var = *self->ports.inName.data;} 
#define setwhile(var, inName) while (true) if(ready(inName)){var = *self->ports.inName.data; break;}
#define anyLogic (moduleLogic)
#define anySetup (moduleSetup)

#define modl(module) module##_Module*

InType(any, void);

/*------------------------------|
       Class Preprocessors	|
-------------------------------*/
#define fn(name, ...) (*name)(__VA_ARGS__)
#define namespace(name, ...) struct {__VA_ARGS__} name;
#define shortName(dotPath, name) typeof(dotPath)* name = &dotPath;
#define using_namespace(name, localName) typeof(name)* localName = &name;

#define method(Class,name, ...) (*name)(Class##_Instance* self __VA_ARGS__)
#define imethod(name, ...) (*name)(inst(Object) object __VA_ARGS__)
#define vmethod(name, ...) (*name)(__VA_ARGS__);

#define methodimpl(Class,Routine, ...) 			\
	Class##_##Routine(Class##_Instance* self __VA_ARGS__)

#define imethodimpl(Class,Routine, ...) 		\
	Class##_##Routine(inst(Object) object __VA_ARGS__)

#define vmethodimpl(Class,Routine, ...) 		\
	Class##_##Routine(__VA_ARGS__)

#define __INIT(...) __VA_ARGS__;
#define __FIELD(...) __VA_ARGS__;
#define __METHODS(...) __VA_ARGS__

#define interface(Class) Class##_Interface Class
#define inhert(Class) Class##_Instance Class

#define interfaceAs(Class) Class##_Interface 
#define inhertAs(Class) Class##_Instance 

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

#define priv self->__private
#define private(name, ...) \
typedef struct name##_Private{__VA_ARGS__}name##_Private; \

#define setpriv(Class) *self->__private = (Class##_Private)

#define self(Class) inst(Class) self = class(Class) object;

#define passover *self = args; return self;

#define asClass(type)							\
	static const u64 sizeof_##type##_Private = 0;			\
	typedef type type##_Instance; 					\
	typedef type type##_ConstructArgs;				\
	static type##_Instance* type##_Construct(			\
		type##_ConstructArgs args,				\
		type##_Instance* self)					\

#define asClassExt(type, construct_args)				\
	static const u64 sizeof_##type##_Private = 0;			\
	typedef type type##_Instance; 					\
	typedef struct {construct_args;} type##_ConstructArgs;		\
	static type##_Instance* type##_Construct(			\
		type##_ConstructArgs args,				\
		type##_Instance* self)					\


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
	name##_Instance* name##_Construct(				\
		name##_ConstructArgs args,				\
		name##_Instance* self);					\

#define __GLOBAL_METHODS__
#include "config.h"
#undef __GLOBAL_METHODS__

#define Class(name,__INIT, __FIELD, ...) 				\
	typedef struct name##_Private name##_Private;			\
	extern const u64 sizeof_##name##_Private;			\
	typedef struct name##_Instance name##_Instance; 		\
	typedef struct name##_ConstructArgs				\
	{__INIT} name##_ConstructArgs;					\
	Interface(name, CORE_METHODS(name) __VA_ARGS__) 		\
	extern name##_Interface name;					\
	typedef struct name##_Instance{					\
		name##_Private* __private;				\
		const name##_Interface* __methods;			\
		__FIELD							\
		u8 __init : 1;						\
	}name##_Instance;						\
	name##_Instance* name##_Construct(				\
		name##_ConstructArgs args,				\
		name##_Instance* self);					\
	InType(name, inst(name)); OutType(name, inst(name))

#define isinit(object) (object->__init)

typedef void* inst;
#define inst(Class) Class##_Instance*
#define intf(Class) const Class##_Interface*
#define data(Class) Class##_Instance
#define ifob(Class) struct{intf(Class) intf; inst(Object) obj;}

#define generic (void*)
#define class(type) (inst(type))
#define like(var) (typeof(var))

#define initialize(name, ptr, ...) name##_Construct((name##_ConstructArgs){__VA_ARGS__}, ptr)

#define new(name, ...) initialize(name, malloc(sizeof(name##_Instance) + sizeof_##name##_Private), __VA_ARGS__)
#define push(name, ...) initialize(name, alloca(sizeof(name##_Instance) + sizeof_##name##_Private), __VA_ARGS__)

#define del(...) { pntr delObjs[] = {__VA_ARGS__}; size_t delObjsNum = sizeof((pntr[]){__VA_ARGS__}); loop(i, delObjsNum) \
		 if((class(Object) delObjs[i])->__methods->__DESTROY == NULL) ERR(ERR_NULLPTR,"no destructor specified for this object"); \
		    else if((class(Object) delObjs[i])->__methods->__DESTROY(generic delObjs[i]) == ERR_NONE) free(delObjs[i]); }
#define pop(...) { pntr delObjs[] = {__VA_ARGS__}; size_t delObjsNum = sizeof((pntr[]){__VA_ARGS__}); loop(i, delObjsNum) \
		 if((class(Object) delObjs[i])->__methods->__DESTROY == NULL) ERR(ERR_NULLPTR,"no destructor specified for this object"); \
		    else (class(Object) delObjs[i])->__methods->__DESTROY(generic (delObjs[i])); }
/*----------------------|
       Base Types	|
----------------------*/

#define Type(name, ...) 						\
	typedef struct name name; 					\
	typedef struct name{__VA_ARGS__}name; 				\
	asClass(name){ passover }					\
	InType(name, name);						\
	OutType(name, name);


#define Data(name, __INIT,...) 						\
	typedef struct name name; 					\
	typedef struct name{__VA_ARGS__}name;				\
	InType(name, name);						\
	OutType(name, name);						\
	asClassExt(name, __INIT)					

#define Enum(name, ...) 						\
	typedef enum {__VA_ARGS__} name; 				\
	asClass(name){ passover }

#define State(name, __INIT, ...) 					\
	typedef enum {__VA_ARGS__} 					\
	name; asClassExt(name, __INIT)


typedef uint64_t u64;
asClass(u64){ passover }
InType(u64, u64);
OutType(u64, u64);

typedef uint32_t u32;
asClass(u32){ passover }
InType(u32, u32);
OutType(u32, u32);

typedef uint16_t u16;
asClass(u16){ passover }
InType(u16, u16);
OutType(u16, u16);

typedef uint8_t u8;
asClass(u8){ passover }
InType(u8, u8);
OutType(u8, u8);

typedef int64_t i64;
asClass(i64){ passover }
InType(i64, i64);
OutType(i64, i64);

typedef int32_t i32;
asClass(i32){ passover }
InType(i32, i32);
OutType(i32, i32);

typedef int16_t i16;
asClass(i16){ passover }
InType(i16, i16);
OutType(i16, i16);

typedef int8_t i8;
asClass(i8){ passover }
InType(i8, i8);
OutType(i8, i8);

asClass(float){ passover }
InType(float, float);
OutType(float, float);

asClass(double){ passover }
InType(double, double);
OutType(double, double);

asClass(bool){ passover }
InType(bool, bool);
OutType(bool, bool);

typedef struct{ char as_cchar; } c8;
asClassExt(c8, char cchar){ self->as_cchar = args.cchar; return self; };
InType(c8, c8);
OutType(c8, c8);

typedef char16_t c16;
asClass(c16){ passover }
InType(c16, c16);
OutType(c16, c16);

typedef char32_t c32;
asClass(c32){ passover }
InType(c32, c32);
OutType(c32, c32);

typedef wchar_t  rune;
asClass(rune){ passover }
InType(rune, rune);
OutType(rune, rune);

typedef char* cstr;
asClassExt(cstr, __INIT(cstr txt; u64 len))
{ *self = calloc(args.len, sizeof(char)); strncpy(*self, args.txt, args.len); return self;}
InType(cstr, cstr);
OutType(cstr, cstr);

typedef wchar_t* wstr;
asClass(wstr){ passover }
InType(wstr, wstr);
OutType(wstr, wstr);

typedef void* pntr;
#define arry(type) type*
#define pntr_shift(ptr, shift_amt) ptr = ((void*)(&(((uint8_t*)ptr)[shift_amt])))
#define pntr_shiftcpy(ptr, shift_amt) ((void*)(&(((uint8_t*)ptr)[shift_amt])))
#define pntr_asVal(addr) (*(uint64_t*)&addr)
asClass(pntr){ passover }
InType(pntr, pntr);
OutType(pntr, pntr);

typedef u32 errvt;
asClass(errvt){ passover }
InType(errvt, errvt);
OutType(errvt, errvt);

typedef void noFail;

typedef i32 eventID;
asClass(eventID){ passover }
InType(eventID, eventID);
OutType(eventID, eventID);

Data(Event, 
__INIT(eventID id; void* data),
     eventID id;
     void* data;
);


#define atom(type) _Atomic(type)

typedef atom(u8)  mutex;
typedef atom(u32) semaphore;

#define busy(mutex,...) if(mutex){errvt err = ERR(ERR_BUSY, "data is currently in use elsewhere"); __VA_ARGS__;} mutex = true; \
			for(atom(u8)* busy = &(mutex); *busy; *busy = false) 

#define busy_return *busy = false; return 

#define while_busy(call) for(call; errnm == ERR_BUSY; call)

#define maxof(type) 	\
_Generic(((type){0}), 	\
i32:    INT32_MAX,  	\
u32:    INT32_MAX,  	\
i64:    INT64_MAX,  	\
u64:    INT64_MAX,  	\
i16:    INT16_MAX, 	\
u16:    INT16_MAX, 	\
i8:     INT8_MAX,	\
u8:     INT8_MAX,	\
float:  FLT_MAX, 	\
double: DBL_MAX,	\
void*:  UINTPTR_MAX  	\
)


#define minof(type) 	\
_Generic(((type){0}),  	\
i32:    INT32_MIN,  	\
u32:    (0),  		\
i64:    INT64_MIN,  	\
u64:    (0),  		\
i16:    INT16_MIN, 	\
u16:    (0),	 	\
i8:     INT8_MIN,	\
u8:     (0),		\
float:  FLT_MIN, 	\
double: DBL_MIN,	\
void*:  (0)  		\
)


Decl(Object);
Class(Object,__INIT(void* private; Object_Interface* methods),,);

Blueprint(Module,,);
typedef void(*moduleLogic)(Module_Module*, void*);
typedef errvt(*moduleSetup)(Module_Module*, void*);


/**
@class IterableList
@brief interface for any object that can be used in the foreach macros
@details each method is called exactly once then is iterated on
*/
Interface(IterableList,

/**
@return the number of items in the item array
*/
	u64   imethod(Size);
/**
@return the pointer to the item at INDEX, if NULL is returned then
it is treated as an error and the foreach loop will end
*/
	void* imethod(ItemAt,, u64 index);
)

/**
@def foreach_pntr(iterable, type, var)
@param iterable any object who's class has implemented the IterableList interface
@param type the type of a single member within the iterable object
@param var the name of the pointer which points to the current member of the iteration
*/
#define foreach_pntr(iterable, type, var) 					\
	typeof(iterable) var##_iterable = iterable;				\
	u64 __##var##_size = var##_iterable->__methods->			\
		IterableList.Size(generic var##_iterable); 			\
	type* var = var##_iterable->__methods->					\
		IterableList.ItemAt(generic var##_iterable, 0);			\
	if(var) for(size_t var##_iterator = 0; 					\
     	    var##_iterator < __##var##_size; 					\
	    var = var##_iterable->__methods->					\
		IterableList.ItemAt(generic var##_iterable, ++var##_iterator)	\
	)
/**
@def foreach(iterable, type, var)
@param iterable any object who's class has implemented the IterableList interface
@param type the type of a single member within the iterable object
@param var the name of the varible which stores the current member of the iteration
*/
#define foreach(iterable, type, var) 						\
	typeof(iterable) var##_iterable = iterable;				\
	u64 __##var##_size = var##_iterable->__methods->			\
		IterableList.Size(generic var##_iterable); 			\
	type var = *(type*)var##_iterable->__methods->				\
		IterableList.ItemAt(generic var##_iterable, 0);			\
	for(size_t var##_iterator = 0; 						\
     	    var##_iterator < __##var##_size; 					\
	    var = *(type*)iterable->__methods->					\
		IterableList.ItemAt(generic var##_iterable, ++var##_iterator)	\
	)


#define boolToObject(var) 		\
	(&(data(Boolean)){ 		\
		NULL, 			\
		&Boolean, 		\
		*(u8*)&			\
		(typeof(var)){var} 	\
	})


#define pntrToObject(var) 		\
	(&(data(Pointer)){ 		\
		NULL, 			\
		&Pointer, 		\
		*(void**)&		\
		(typeof(var)){var} 	\
	})

#define charToObject(var) 		\
	(&(data(Char)){ 		\
		NULL, 			\
		&Char, 			\
		{getCharType(var), 	\
		*(wchar_t*)&		\
		(typeof(var)){var}} 	\
	})

#define cstrToObject(var) 		\
	(&(data(CString)){ 		\
		NULL, 			\
		&CString, 		\
		{getCharType(var), 	\
		*((void**)&		\
		(typeof(var)){var})} 	\
	})

#define floatToObject(var) 		\
	(&(data(Float)){ 		\
		NULL, 			\
		&Float, 		\
		*(double*)&		\
		(typeof(var)){var}, 	\
		.dbl = _Generic(var, 	\
		float  : false,		\
		double : true,		\
	  	default: false		\
	   	)			\
	})

#define intToObject(var) 		\
	(&(data(Integer)){ 		\
		NULL, 			\
		&Integer, 		\
		*(u64*)&		\
		(typeof(var)){var}, 	\
		.sign = _Generic(var, 	\
		i32 : true, u32 : false,\
		i64 : true, u64 : false,\
		i16 : true, u16 : false,\
		i8  : true, u8  : false,\
	  	default: false		\
	   	),			\
		.longint = _Generic(var,\
		i32 : false, u32 :false,\
		i64 : true,  u64 :true, \
		i16 : false, u16 :false,\
		i8  : false, u8  :false,\
	  	default: false		\
	   	)			\
	})

#define asObject(var)			\
_Generic((var), 			\
i32:    	intToObject(var),	\
u32:		intToObject(var), 	\
i64:    	intToObject(var), 	\
u64:    	intToObject(var), 	\
i16:   		intToObject(var), 	\
u16:   		intToObject(var),	\
i8:   		intToObject(var),	\
u8:   		intToObject(var),	\
float:   	floatToObject(var),	\
double:   	floatToObject(var),	\
const char*: 	cstrToObject(var),	\
char*:	 	cstrToObject(var),	\
c8:	 	charToObject(var),	\
char:	 	charToObject(var),	\
void*:	  	pntrToObject(var),	\
bool:		boolToObject(var), 	\
default: 	var)		

#define getMethods(type)  	\
_Generic(((data(type)){0}), 	\
i32:    	Integer,	\
u32:		Integer,	\
i64:    	Integer,	\
u64:    	Integer,	\
i16:   		Integer,	\
u16:   		Integer,	\
u8:   		Integer,	\
i8:   		Integer,	\
float:   	Float,		\
double:  	Float,		\
const char*: 	CString,	\
cstr:	  	CString,	\
pntr: 	 	Pointer,	\
bool:		Boolean,	\
default: 	type)		

#pragma once
#define __XC_TYPES__
#include "../pkg.h"

//=======EXPORTS========

#define package std

#include "utils.h"
#include "__macro_utils.h"


/*======================|
 *   	 Types
======================*/

#define __CONCATE_IMPL(name, othername) name##_##othername
#define ___(...) __MODULE_CAT_NAME(__VA_ARGS__)

#define INIT(...) __VA_ARGS__;
#define FMT(...) __VA_ARGS__
#define DEF(...) __VA_ARGS__

#define __FROM_IMPL(package, type, alias) 					\
    static const char* alias##_TypeID = package##_##type##_TypeID;		\
    static const package##_##type##_Type_t* alias##_Type = 			\
	    &package##_##type##_TypeData;					\
    typedef struct package##_##type##_Interface alias##_Interface;		\
    typedef struct package##_##type##_Interface_Obj alias##_Interface_Obj;	\
    typedef struct package##_##type##_Obj alias##_Obj;				\
    typedef package##_##type##_ConstructArgs alias##_ConstructArgs;		\
    typedef package##_##type##_FormatArgs alias##_FormatArgs;			\
    typedef package##_##type alias;

#define as ,
#define use(type) type as type

#define from(package, ...) \
    __FROM_FOR_EACH_DISPATCH(__FROM_IMPL, package, __VA_ARGS__)

#define pkg(name) ___(package, name) 
#define mod(name) ___(module,  name) 


#define enum(name, ...)							\
	typedef enum {__VA_ARGS__} ___(package, name); 			\

#define Decl(name) 										\
	extern const char ___(package, name##_TypeID)[];					\
	typedef struct ___(package, name) ___(package, name);					\
	typedef struct ___(package, name##_ConstructArgs) ___(package, name##_ConstructArgs);	\
	typedef struct ___(package, name##_FormatArgs) ___(package, name##_FormatArgs);		\
	typedef struct ___(package, name##_Ops_t) ___(package, name##_Ops_t);			\
	typedef struct ___(package, name##_Type_t) ___(package, name##_Type_t);			\
	typedef const struct ___(package, name##_Interface) ___(package, name##_Interface);	\
	typedef struct ___(package, name##_Interface) ___(package, name##_VTable_Interface);	\
	typedef struct ___(package, name##_Interface_Obj) ___(package, name##_Interface_Obj);	\
	extern const ___(package,name##_Type_t) ___(package,name##_TypeData);  			\



#define defTypeID(type) const char type##_TypeID[] = #type;

#define typefrom(type, name, ...) 						\
	extern const char ___(package, name##_TypeID)[];			\
	typedef struct ___(package, type##_FormatArgs) 				\
	___(package, name##_FormatArgs);					\
	typedef type ___(package, name);					\
	typedef ___(package, name) ___(package, name##_ConstructArgs);		\
	typedef struct ___(package,name##_Ops_t)				\
	{OPERATOR_FUNCS(___(package, name))}___(package,name##_Ops_t);		\
	typedef struct ___(package,name##_Type_t){				\
		___(package,name##_Ops_t)* ops;					\
		const len_t size;						\
		const char* id;							\
		const ___(package, name##_FormatArgs)* format;			\
		const ___(package, name##_ConstructArgs)* construct;		\
	}___(package,name##_Type_t);  						\
	extern const type##_FormatArgs type##_DefaultFormat;  			\
	extern const type##_Ops_t type##_OpsImpl;  				\
	extern const type##_ConstructArgs type##_DefaultArgs;  			\
	static const ___(package,name##_Type_t) ___(package,name##_TypeData) = {\
		generic &type##_OpsImpl, 					\
		sizeof(___(package,name)),					\
		generic type##_TypeID, 						\
		generic &type##_DefaultFormat, 					\
		generic &type##_DefaultArgs };					\
	static const ___(package,name##_Type_t)* ___(package,name##_Type) =  	\
		&___(package,name##_TypeData) ;


#define type_fn(type, name, ...) 							\
	typedef type (*___(package, name))(__VA_ARGS__);				\
	typedef ___(package, name) ___(package, name##_ConstructArgs);			\
	typedef struct ___(package, name##_FormatArgs) ___(package, name##_FormatArgs);	\
	extern const char ___(package, name##_TypeID)[];				\
	typedef struct ___(package,name##_Ops_t) {OPERATOR_FUNCS(___(package, name))}	\
	___(package,name##_Ops_t);							\
	extern const ___(package,name##_Ops_t) ___(package,name##_OpsImpl); 		\
	typedef struct ___(package,name##_Type_t){					\
		___(package,name##_Ops_t)* ops;						\
		const len_t size;							\
		const char* id;								\
		const ___(package, name##_FormatArgs)* format;				\
		const ___(package, name##_ConstructArgs)* construct;			\
	}___(package,name##_Type_t);  							\
	extern const ___(package,name##_Type_t) ___(package,name##_TypeData);  		\
	static const ___(package,name##_Type_t)* ___(package,name##_Type) = 		\
		&___(package,name##_TypeData);  		

#define type_array(type, name, len) 							\
	typedef type ___(package, name)[len];						\
	typedef ___(package, name) ___(package, name##_ConstructArgs);			\
	typedef struct ___(package, name##_FormatArgs) ___(package, name##_FormatArgs);	\
	extern const char ___(package, name##_TypeID)[];				\
	typedef struct ___(package,name##_Ops_t) {OPERATOR_FUNCS(___(package, name))}	\
	___(package,name##_Ops_t);							\
	extern const ___(package,name##_Ops_t) ___(package,name##_OpsImpl); 		\
	typedef struct ___(package,name##_Type_t){					\
		___(package,name##_Ops_t)* ops;						\
		const len_t size;							\
		const char* id;								\
		const ___(package, name##_FormatArgs)* format;				\
		const ___(package, name##_ConstructArgs)* construct;			\
	}___(package,name##_Type_t);  							\
	extern const ___(package,name##_Type_t) ___(package,name##_TypeData);  		\
	static const ___(package,name##_Type_t)* ___(package,name##_Type) = 		\
		&___(package,name##_TypeData);  		


#define type(name, ...) 								\
	typedef struct ___(package, name) ___(package, name); 				\
	typedef struct ___(package, name) ___(package, name##_ConstructArgs);		\
	typedef struct ___(package, name) ___(package, name##_FormatArgs);		\
	extern const char ___(package, name##_TypeID)[];				\
	typedef struct ___(package,name##_Ops_t) {OPERATOR_FUNCS(___(package, name))}	\
	___(package,name##_Ops_t);							\
	extern const ___(package,name##_Ops_t) ___(package,name##_OpsImpl); 		\
	typedef struct ___(package, name) {__VA_ARGS__} ___(package, name); 		\
	typedef struct ___(package,name##_Type_t){					\
		___(package,name##_Ops_t)* ops;						\
		const len_t size;							\
		const char* id;								\
		const ___(package, name##_FormatArgs)* format;				\
		const ___(package, name##_ConstructArgs)* construct;			\
	}___(package,name##_Type_t);  							\
	extern const ___(package,name##_Type_t) ___(package,name##_TypeData);  		\
	static const ___(package,name##_Type_t)* ___(package,name##_Type) = 		\
		&___(package,name##_TypeData);  		

#define asXCType(type)						\
	Decl(type)						\
	extern const char type##_TypeID[];			\
	typedef type type##_Instance; 				\
	typedef type type##_ConstructArgs;			\
	typedef type type##_FormatArgs;				\
	typedef struct type##_Ops_t {OPERATOR_FUNCS(type)}	\
	type##_Ops_t;						\
	typedef struct type##_Type_t{				\
		const type##_Ops_t* ops;			\
		const len_t size;				\
		const char* id;					\
		const type##_FormatArgs* format;		\
		const type##_ConstructArgs* construct;		\
	}type##_Type_t;  					\
	extern const type##_Type_t type##_TypeData;  		\
	static const type##_Type_t* 				\
	type##_Type = &type##_TypeData; 			\

#define asXCType_INIT(type, ...)				\
	Decl(type)						\
	extern const char type##_TypeID[];			\
	typedef type type##_Instance; 				\
	typedef struct {__VA_ARGS__;} type##_ConstructArgs;	\
	typedef type type##_FormatArgs;				\
	typedef struct type##_Ops_t {OPERATOR_FUNCS(type)}	\
	type##_Ops_t;						\
	typedef struct type##_Type_t{				\
		const type##_Ops_t* ops;			\
		const len_t size;				\
		const char* id;					\
		const type##_FormatArgs* format;		\
		const type##_ConstructArgs* construct;		\
	}type##_Type_t;  					\
	extern const type##_Type_t type##_TypeData;  		\
	static const type##_Type_t* 				\
	type##_Type = &type##_TypeData; 			\


#define asXCType_FMT(type, ...)					\
	Decl(type)						\
	extern const char type##_TypeID[];			\
	typedef type type##_Instance; 				\
	typedef type type##_ConstructArgs;			\
	typedef struct {__VA_ARGS__;}  type##_FormatArgs;	\
	typedef struct type##_Ops_t {OPERATOR_FUNCS(type)}	\
	type##_Ops_t;						\
	typedef struct type##_Type_t{				\
		const type##_Ops_t* ops;			\
		const len_t size;				\
		const char* id;					\
		const type##_FormatArgs* format;		\
		const type##_ConstructArgs* construct;		\
	}type##_Type_t;  					\
	extern const type##_Type_t type##_TypeData;  		\
	static const type##_Type_t* 				\
	type##_Type = &type##_TypeData; 			\


#define asClass(type, INIT, FMT)							\
	Decl(type);									\
	extern const char ___(package,  type##_TypeID)[];				\
	typedef struct ___(package,type##_Ops_t) {OPERATOR_FUNCS(___(package, type))}	\
	___(package,type##_Ops_t);							\
	extern const ___(package,type##_Ops_t) 						\
	___(package,type##_OpsImpl); 							\
	typedef struct ___(package,type##_Type_t){					\
		___(package,type##_Ops_t)* ops;						\
		const len_t size;							\
		const char* id;								\
		const ___(package, type##_FormatArgs)* format;				\
		const ___(package, type##_ConstructArgs)* construct;			\
	}___(package,type##_Type_t);  							\
	extern const ___(package,type##_Type_t) ___(package,type##_TypeData);  		\
	static const ___(package,type##_Type_t)* ___(package,type##_Type) = 		\
		&___(package,type##_TypeData);  					\
	typedef ___(package, type) ___(package, type##_Instance); 			\
	typedef struct {INIT} ___(package, type##_ConstructArgs);			\
	typedef struct ___(package, name##_FormatArgs)  {FMT;} 				\
	___(package, name##_FormatArgs);						\
	CREATE(___(package, type))


#undef Interface

#define __INTERFACE_OBSCURE(...)									\
	typedef struct  ___(package __VA_OPT__(, __VA_ARGS__), Interface) 				\
			___(package __VA_OPT__(, __VA_ARGS__), Interface_ConstructArgs);		\
	typedef struct  ___(package __VA_OPT__(, __VA_ARGS__), Interface)				\
			___(package __VA_OPT__(, __VA_ARGS__), Interface_FormatArgs);			\
	typedef struct  ___(package __VA_OPT__(, __VA_ARGS__), Interface_Ops_t) 			\
			{OPERATOR_FUNCS(___(package __VA_OPT__(, __VA_ARGS__), Interface))}		\
		       ___(package __VA_OPT__(, __VA_ARGS__),  Interface_Ops_t);			\
	typedef struct ___(package __VA_OPT__(, __VA_ARGS__), Interface_Type_t){			\
		___(package __VA_OPT__(, __VA_ARGS__), Interface_Ops_t)* ops;				\
		const len_t size;									\
		const char* id;										\
		const ___(package __VA_OPT__(, __VA_ARGS__), Interface_FormatArgs)* format;		\
		const ___(package __VA_OPT__(, __VA_ARGS__), Interface_ConstructArgs)* construct;	\
	}___(package __VA_OPT__(, __VA_ARGS__), Interface_Type_t);  					\
		 											\
	static const char ___(package __VA_OPT__(, __VA_ARGS__), Interface_TypeID)[] = STRINGIFY(	\
		       	  ___(package __VA_OPT__(, __VA_ARGS__), Interface));				\
	static const ___(package __VA_OPT__(, __VA_ARGS__), Interface_Ops_t) 				\
		     ___(package __VA_OPT__(, __VA_ARGS__), Interface_OpsImpl) = {0};			\
	static const ___(package __VA_OPT__(, __VA_ARGS__), Interface_Type_t) 				\
		     ___(package __VA_OPT__(, __VA_ARGS__), Interface_TypeData) = {0};			\
	static const ___(package __VA_OPT__(, __VA_ARGS__), Interface_Type_t)* 				\
		     ___(package __VA_OPT__(, __VA_ARGS__), Interface_Type) = 				\
		    &___(package __VA_OPT__(, __VA_ARGS__), Interface_TypeData);

#define __INTERFACE_OBJECT(...)										\
	typedef struct ___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj) {				\
		const  ___(package __VA_OPT__(, __VA_ARGS__), Interface)* interface; 			\
		pntr object;										\
	}___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj);						\
	typedef struct  ___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj) 				\
			___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj_ConstructArgs);		\
	typedef struct  ___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj) 				\
			___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj_FormatArgs);		\
	typedef struct  ___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj_Ops_t) 			\
			{OPERATOR_FUNCS(___(package __VA_OPT__(, __VA_ARGS__) , Interface_Obj))}		\
	___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj_Ops_t);					\
	typedef struct ___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj_Type_t){			\
	    ___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj_Ops_t)* ops;				\
	    const len_t size;										\
	    const char* id;										\
	    const ___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj_FormatArgs)* format;		\
	    const ___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj_ConstructArgs)* construct;	\
	}___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj_Type_t);  					\
													\
	static const char ___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj##_TypeID)[] = 		\
		STRINGIFY(___(package __VA_OPT__(, __VA_ARGS__), Interface));				\
	static const ___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj_Ops_t) 			\
		     ___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj_OpsImpl) = {0};		\
	static const ___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj_Type_t) 			\
		     ___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj_TypeData) = {0};		\
	static const ___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj_Type_t)* 			\
		 ___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj_Type) = 				\
		&___(package __VA_OPT__(, __VA_ARGS__), Interface_Obj_TypeData);				\


#define Interface(name, ...) 										\
	typedef const struct ___(package, name##_Interface) ___(package, name##_Interface);		\
	typedef struct ___(package, name##_Interface) ___(package, name##_VTable_Interface);		\
	__INTERFACE_OBSCURE(name); 									\
	__INTERFACE_OBJECT(name);									\
	__INTERFACE_OBSCURE(name##_VTable); 								\
	__INTERFACE_OBJECT(name##_VTable);								\
	struct ___(package,name##_Interface){__VA_ARGS__};

#define Interface_Pkg(...) 										\
	typedef const struct ___(package, Interface) ___(package, Interface);				\
	typedef struct ___(package, Interface) ___(package, VTable_Interface);				\
	__INTERFACE_OBSCURE(); 										\
	__INTERFACE_OBJECT();										\
	__INTERFACE_OBSCURE(VTable); 									\
	__INTERFACE_OBJECT(VTable);									\
	struct ___(package, Interface){__VA_ARGS__};

#define private(...) struct {__VA_ARGS__} __private;  					\

#define Data(name, INIT, FMT, ...) 							\
	Decl(name);									\
	typedef struct ___(package, name) ___(package, name); 				\
	typedef struct ___(package, name){ 						\
	const ___(package,name##_Type_t)* __type; __VA_ARGS__}___(package, name);	\
	extern const char ___(package,  name##_TypeID)[];				\
	typedef struct ___(package,name##_Ops_t) {OPERATOR_FUNCS(___(package, name))}	\
	___(package,name##_Ops_t);							\
	extern const ___(package,name##_Ops_t) 						\
	___(package,name##_OpsImpl); 							\
	typedef struct ___(package,name##_Type_t){					\
		___(package,name##_Ops_t)* ops;						\
		const len_t size;							\
		const char* id;								\
		const ___(package, name##_FormatArgs)* format;				\
		const ___(package, name##_ConstructArgs)* construct;			\
	}___(package,name##_Type_t);  							\
	extern const ___(package,name##_Type_t) ___(package,name##_TypeData);  		\
	static const ___(package,name##_Type_t)* ___(package,name##_Type) = 		\
		&___(package,name##_TypeData);  					\
	typedef ___(package, name) ___(package, name##_Instance); 			\
	typedef struct ___(package, name##_ConstructArgs) {INIT;} 			\
	___(package, name##_ConstructArgs);						\
	typedef struct ___(package, name##_FormatArgs)  {FMT;} 				\
	___(package, name##_FormatArgs);						\
	typedef struct ___(package, name##_FormatArgs) ___(package, name##_FormatArgs);	\




#define Class(name, INIT, FMT, ...) 								\
	Data(name, DEF(INIT), DEF(FMT), __VA_ARGS__);						\
	typedef const struct ___(package, name##_Interface) ___(package, name##_Interface);	\
	typedef struct ___(package, name##_Interface) ___(package, name##_VTable_Interface);	\
	__INTERFACE_OBSCURE(name)								\
	__INTERFACE_OBJECT(name)								\
	__INTERFACE_OBSCURE(name##_VTable)							\
	__INTERFACE_OBJECT(name##_VTable)							\
	struct ___(package, name##_Interface) 


#define constructor(Class, name, ...)								\
	typedef struct {__VA_ARGS__;} ___(package, Class##_CONSTRUCTOR_##name##_ConstructArgs);	\
	typedef struct ___(package, Class##_CONSTRUCTOR_##name##_Ops_t) 			\
	{___(package, Class)* fn(Create)(___(package, Class)*,len_t, 				\
	___(package, Class##_CONSTRUCTOR_##name##_ConstructArgs)*);} 				\
     		___(package,Class##_CONSTRUCTOR_##name##_Ops_t);				\
	extern const ___(package,Class##_CONSTRUCTOR_##name##_Ops_t)				\
	___(package, Class##_CONSTRUCTOR_##name##_OpsImpl); 					\
	typedef struct ___(package, Class##_CONSTRUCTOR_##name##_Type_t){			\
		___(package,Class##_CONSTRUCTOR_##name##_Ops_t)* ops;				\
		const len_t size;								\
		const char* id;									\
		const ___(package, Class##_FormatArgs)* format;					\
		const ___(package, Class##_CONSTRUCTOR_##name##_ConstructArgs)* construct;	\
	}___(package,Class##_CONSTRUCTOR_##name##_Type_t);  					\
	extern const ___(package,Class##_CONSTRUCTOR_##name##_Type_t) 				\
     		___(package,Class##_CONSTRUCTOR_##name##_TypeData);  				\
	static const ___(package,Class##_CONSTRUCTOR_##name##_Type_t)* 				\
		___(package,Class##_CONSTRUCTOR_##name##_Type) = 				\
		&___(package,Class##_CONSTRUCTOR_##name##_TypeData);  				


#undef package

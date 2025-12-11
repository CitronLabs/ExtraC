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
#ifdef __PKG
	#define ___(a,b) __CONCATE_IMPL(PKG_##a,b)
#else
	#define ___(a,b) __CONCATE_IMPL(a,b)
#endif
#define INIT(...) __VA_ARGS__;
#define FMT(...) __VA_ARGS__
#define DEF(...) __VA_ARGS__

#define __FROM_IMPL(package, type, alias) 				\
    static const char* alias##_TypeID = package##_##type##_TypeID;	\
    static const package##_##type##_Type_t* alias##_Type = 		\
	    &package##_##type##_TypeData;				\
    typedef struct package##_##type##_Interface alias##_Interface;	\
    typedef package##_##type##_ConstructArgs alias##_ConstructArgs;	\
    typedef package##_##type##_FormatArgs alias##_FormatArgs;		\
    typedef package##_##type alias;

#define as ,
#define use(type) type as type

#define from(package, ...) \
    __FROM_FOR_EACH_DISPATCH(__FROM_IMPL, package, __VA_ARGS__)

#define enum(name, ...)							\
	typedef enum {__VA_ARGS__} ___(package, name); 			\

#define Decl(name) 										\
	extern const char ___(package, name##_TypeID)[];					\
	typedef struct ___(package, name) ___(package, name);					\
	typedef struct ___(package, name##_ConstructArgs) ___(package, name##_ConstructArgs);	\
	typedef struct ___(package, name##_FormatArgs) ___(package, name##_FormatArgs);		\
	typedef struct ___(package, name##_Ops_t) ___(package, name##_Ops_t);			\
	typedef struct ___(package, name##_Type_t) ___(package, name##_Type_t);			\
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
	extern const char ___(package, name##_TypeID)[];					\
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
	extern const char ___(package, name##_TypeID)[];					\
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
	extern const char ___(package, name##_TypeID)[];					\
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
	extern const char ___(package,  type##_TypeID)[];					\
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

#define Interface(name, ...) 				\
	typedef const struct ___(package,name##_Interface)	\
	{__VA_ARGS__} ___(package,name##_Interface);	\


#define VTable(name, ...) 				\
	typedef struct ___(package,name##_Interface)	\
	{__VA_ARGS__} ___(package,name##_Interface);	\

#define private(...) struct {__VA_ARGS__} __private;  	\

#define Data(name, INIT, FMT, ...) 							\
	Decl(name);									\
	typedef struct ___(package, name) ___(package, name); 				\
	typedef struct ___(package, name){ 						\
	const ___(package,name##_Type_t)* __type; __VA_ARGS__}___(package, name);	\
	extern const char ___(package,  name##_TypeID)[];					\
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
	typedef struct ___(package, name##_ConstructArgs) {INIT} 			\
	___(package, name##_ConstructArgs);						\
	typedef struct ___(package, name##_FormatArgs)  {FMT;} 				\
	___(package, name##_FormatArgs);						\
	typedef struct ___(package, name##_FormatArgs) ___(package, name##_FormatArgs);	\

#define Class(name, INIT, FMT, ...) 							\
	Data(name, DEF(INIT), DEF(FMT), __VA_ARGS__);					\
	typedef struct ___(package,name##_Interface) ___(package,name##_Interface);	\
	struct ___(package,name##_Interface) 





#undef package

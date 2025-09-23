#include "classes.h"

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

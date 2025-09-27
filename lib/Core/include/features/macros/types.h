
#define boolToObject(var) 		\
	(&(data(Boolean)){ 		\
		NULL, 			\
		(void*) &Boolean_Ops, 	\
		*(u8*)&			\
		(typeof(var)){var} 	\
	})


#define pntrToObject(var) 		\
	(&(data(Pointer)){ 		\
		NULL, 			\
		(void*) &Pointer_Ops, 	\
		*(void**)&		\
		(typeof(var)){var} 	\
	})

#define charToObject(var) 		\
	(&(data(Char)){ 		\
		NULL, 			\
		(void*) &Char_Ops,	\
		{getCharType(var), 	\
		*(wchar_t*)&		\
		(typeof(var)){var}} 	\
	})

#define xccharToObject(var) 		\
	(&(data(Char)){ 		\
		NULL, 			\
		(void*) &Char_Ops,	\
  		*(char_t*)var		\
	})

#define cstrToObject(var) 		\
	(&(data(CString)){ 		\
		NULL, 			\
		(void*) &CString_Ops, 	\
		{(void*) &str_t_Ops,	\
		0,getCharType(var), 	\
		*((void**)&		\
		(typeof(var)){var})} 	\
	})
#define xcstrToObject(var) 		\
	(&(data(CString)){ 		\
		NULL, 			\
		(void*) &CString_Ops, 	\
		*(str_t*)var 		\
	}) 				
#define xcbufToObject(var) 		\
	(&(data(Buff)){ 		\
		NULL, 			\
		(void*) &Buff_Ops,	\
		*(buff_t*)var 		\
	}) 				

#define floatToObject(var) 		\
	(&(data(Float)){ 		\
		NULL, 			\
		(void*) &Float_Ops, 	\
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
		(void*)	&Integer_Ops, 	\
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
str_t:		xcstrToObject(var),	\
buff_t:		xcbufToObject(var),	\
c8:	 	charToObject(var),	\
char:	 	charToObject(var),	\
char_t:	 	xccharToObject(var),	\
void*:	  	pntrToObject(var),	\
bool:		boolToObject(var), 	\
default: 	var)		







#define namespace(name, ...) struct {__VA_ARGS__} name;
#define shortName(dotPath, name) typeof(dotPath)* name = &dotPath;
#define using_namespace(name, localName) typeof(name)* localName = &name;

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
u32:    ((u32)0),  	\
i64:    INT64_MIN,  	\
u64:    ((u64)0),  	\
i16:    INT16_MIN, 	\
u16:    ((u16)0),	\
i8:     INT8_MIN,	\
u8:     ((u8)0),	\
float:  FLT_MIN, 	\
double: DBL_MIN,	\
void*:  ((void*)0)  	\
)

#define DO_LATER(...) static_assert(false , __VA_ARGS__);

#define getbitflagsinit size_t currentflag = 0, flagschecked = 0; 

#define getbitflags(flags, ...)											\
	currentflag = 0;											\
	flagschecked = 0;											\
	for (size_t bitmask = 1;(flags - flagschecked) > 0; bitmask*=2) 					\
	{													\
		currentflag = flags & bitmask;									\
		switch (currentflag)										\
		__VA_ARGS__											\
		flagschecked |= currentflag;									\
	}
#define getbitflag(flags, flag) (flags & flag) == flag	



 #define BLACK "\033[0;30m"
 #define RED "\033[0;31m"
 #define GREEN "\033[0;32m"
 #define ORANGE "\033[0;33m"
 #define BLUE "\033[0;34m"
 #define PURPLE "\033[0;35m"
 #define CYAN "\033[0;36m"
 #define LIGHTGRAY "\033[0;37m"   
 #define DARKGRAY "\033[1;30m"   
 #define LIGHTRED "\033[1;31m"   
 #define LIGHTGREEN "\033[1;32m"   
 #define YELLOW "\033[1;33m"   
 #define LIGHTBLUE "\033[1;34m" 
 #define LIGHTPURPLE "\033[1;35m"
 #define LIGHTCYAN "\033[1;36m"
 #define WHITE "\033[1;37m"
 #define NC "\033[1;0m"


#define loop(loopvar, num) for(int loopvar = 0; loopvar < num; loopvar++)

#define loopat(loopvar, init, num) for(int loopvar = init; loopvar < num; loopvar++)

#define iter(array, size, type, item) 	\
type* item; 				\
for(int __##item##_iterator = 0; 	\
    __##item##_iterator < size; 	\
    item = &((type*)array)[__##item##_iterator++])



#define setAllOps(Class) 			\
		.Append  = Class##__Append, 	\
		.Set     = Class##__Set,	\
		.Copy    = Class##__Copy,	\
		.Size    = Class##__Size,	\
		.Hash    = Class##__Hash,	\
		.Destroy = Class##__Destroy,	\
		.Iter    = Class##__Iter,	\
		.Scan    = Class##__Scan,	\
		.Print   = Class##__Print,	\



#define boolToObject(var) 		\
	(&(data(Boolean)){ 		\
		NULL, 			\
		(void*) &Boolean_Ops, 	\
		(bool*)&		\
		(typeof(var)){var} 	\
	})


#define pntrToObject(var) 		\
	(&(data(Pointer)){ 		\
		NULL, 			\
		(void*) &Pointer_Ops, 	\
		(void**)&		\
		(typeof(var)){var} 	\
	})

#define charToObject(var) 		\
	(&(data(Char)){ 		\
		NULL, 			\
		(void*) &Char_Ops,	\
		(wchar_t*)&		\
		(typeof(var)){var} 	\
	})

#define cstrToObject(var) 		\
	(&(data(CString)){ 		\
		NULL, 			\
		(void*) &CString_Ops, 	\
		((wchar_t**)&		\
		(typeof(var)){var}) 	\
	})

#define floatToObject(var) 		\
	(&(data(Float)){ 		\
		NULL, 			\
		(void*) &Float_Ops, 	\
		(double*)&		\
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
		(u64*)&		\
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


#pragma once
#define __XC_TYPES__
#include "../pkg.h"

#include "../types.h"

#define __EVAL_VALUE 0
#define __END_VALUE  1
#define __SKIP_VALUE 2 

#define fmt_eval ((pntr)__EVAL_VALUE)
#define fmt_skip ((pntr)__END_VALUE)
#define fmt_end  ((pntr)__SKIP_VALUE)

#define f (*format)

#define __AS_FMT_OBJ(var) 			\
_Generic((var), 				\
i32:    	&(std_Types_Format_Value){0},	\
u32:		&(std_Types_Format_Value){0}, 	\
i64:    	&(std_Types_Format_Value){0}, 	\
u64:    	&(std_Types_Format_Value){0}, 	\
i16:   		&(std_Types_Format_Value){0}, 	\
u16:   		&(std_Types_Format_Value){0},	\
i8:   		&(std_Types_Format_Value){0},	\
u8:   		&(std_Types_Format_Value){0},	\
float:   	&(std_Types_Format_Value){0},	\
double:   	&(std_Types_Format_Value){0},	\
const char*: 	&(std_Types_Format_Text) {0},	\
char*:	 	&(std_Types_Format_Text) {0},	\
char:	 	&(std_Types_Format_Text) {0},	\
void*:	  	&(std_Types_Format_Value){0},	\
bool:		&(std_Types_Format_Value){0}, 	\
default: 	var)	

#define $(expr)   	  null, generic &asObject((expr))->__type->ops, generic asObject((expr)), NULL
#define $use(typeData, data)   null, generic &((typeData)->ops), generic data, NULL

#define $F(expr, ...)    							\
	null, generic &asObject((expr))->__type->ops, generic asObject((expr)), 	\
	generic &(typeof(*((__AS_FMT_OBJ(expr))->__type->format))){__VA_ARGS__}

#define $Fwith(expr, args)    									\
	null, generic &asObject((expr))->__type->ops, generic asObject((expr)), generic &args	\

#define $useF(typeData, data, ...)   	 				\
	null, generic &(typeData)->ops, generic data, 			\
	&(typeof(*(typeData)->format)){__VA_ARGS__}

#define $useFwith(typeData, data, args)   	 			\
	null, generic &(typeData)->ops, generic data, generic &args	\



#define printTo(strm, ...) 	std.Types.data.writeTo( 			\
				    V(strm), 			\
				    (void*[]){__VA_ARGS__}, 		\
				    __VA_ARG_LEN__(type, __VA_ARGS__) 	\
	  			)


#define scanFrom(strm, ...) 	std.Types.data.readFrom( 		\
                                     V(strm), 			\
                                     (void*[]){__VA_ARGS__}, 		\
                                     __VA_ARG_LEN__(type, __VA_ARGS__) 	\
                                )

#define print(...) 	 printTo(std.Stream.stdOut(),  __VA_ARGS__) 
#define scan(...) 	 scanFrom(std.Stream.stdIn(),  __VA_ARGS__) 
#define println(...) 	 printTo(std.Stream.stdOut(), __VA_ARGS__ , "\n") 
#define scanln(...) 	 scanFrom(std.Stream.stdIn(), __VA_ARGS__ , "\n") 

#define package std_Types_Format

Data(Value,
INIT(),
FMT(len_t precision; double percentOf; byte base),
	
)

Data(Text,
INIT(),
FMT(),
	
)

#undef package


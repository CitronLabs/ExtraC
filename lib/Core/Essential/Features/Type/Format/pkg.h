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

#define $(expr)   	  null, generic &asObject((expr))->__type->ops, generic asObject((expr)), NULL
#define $use(typeData, data)   null, generic &((typeData)->ops), generic data, NULL

#define $F(expr, ...)    							\
	null, generic &asObject((expr))->__type->ops, generic asObject((expr)), 	\
	generic &(typeof(*asObject(expr)->__type->format)){__VA_ARGS__}

#define $Fwith(expr, args)    									\
	null, generic &asObject((expr))->__type->ops, generic asObject((expr)), generic &args	\

#define $useF(typeData, data, ...)   	 				\
	null, generic &(typeData)->ops, generic data, 			\
	&(typeof(*(typeData)->format)){__VA_ARGS__}

#define $useFwith(typeData, data, args)   	 			\
	null, generic &(typeData)->ops, generic data, generic &args	\



#define printTo(strm, ...) 	std.Type.data.writeTo( 			\
				    varDataOf(strm), 			\
				    (void*[]){__VA_ARGS__}, 		\
				    __VA_ARG_LEN__(type, __VA_ARGS__) 	\
	  			)


#define scanFrom(strm, ...) 	std.Type.data.readFrom( 		\
                                     varDataOf(strm), 			\
                                     (void*[]){__VA_ARGS__}, 		\
                                     __VA_ARG_LEN__(type, __VA_ARGS__) 	\
                                )

#define print(...) 	 printTo(std.Stream.stdOut(),  __VA_ARGS__) 
#define scan(...) 	 scanFrom(std.Stream.stdIn(),  __VA_ARGS__) 
#define println(...) 	 printTo(std.Stream.stdOut(), __VA_ARGS__ , "\n") 
#define scanln(...) 	 scanFrom(std.Stream.stdIn(), __VA_ARGS__ , "\n") 


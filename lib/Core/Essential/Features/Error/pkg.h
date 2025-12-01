#pragma once
#define __XC_FEATURE__
#include "../pkg.h"

#include "../Type/pkg.h"

#define package std


#define OK ERR_NONE

#define ERR(code, msg) std.Error.Set(&(std_Error){code, msg}, #code, __func__)

#define check(...) for(std_Error* err = std.Error.Get(); err->errorcode != ERR_NONE; std.Error.Clear()) 	\
		   loop(i, 										\
	 		(sizeof((errvt[]){__VA_ARGS__}) / sizeof(errvt)) ? 				\
	 		(sizeof((errvt[]){__VA_ARGS__}) / sizeof(errvt)) : 1				\
		   ) if ((sizeof((errvt[]){__VA_ARGS__}) / sizeof(errvt)) ? 				\
    			 ((errvt[]){__VA_ARGS__})[i] == err->errorcode : err->errorcode != ERR_NONE)

#define try(...) std.Error.Clear(); 									\
		if(!std.Error.Try((errvt[]){__VA_ARGS__}, sizeof((errvt[]){__VA_ARGS__})/sizeof(errvt)))	\

#define catch else for(std_Error* err = std.Error.Get(); err->errorcode != ERR_NONE; std.Error.Clear())

#define throw(code, msg) ERR(code, msg); std.Error.Throw();
#define nullerr(var) ERR(ERR_nullPTR, #var " is null")
#define nonull(var, ...) if(!(var)){errvt err = nullerr(var); __VA_ARGS__;}
#define iferr(errorable) for(errvt err = (errorable); err; std.Error.Clear())
#define NOT_IMPLEM(returnval) ERR(ERR_NOTIMPLEM, "not implemented yet..."); return returnval;

#define errnm  (std.Error.Get()->errorcode)
#define errstr (std.Error.Get()->message)

typedef enum{ 
	#define __ERROR_CODES__
	#include "./config.c"
	#undef __ERROR_CODES__
}XC_ERROR_CODES;

type(Error,
	errvt errorcode; 
	char* message;
);


Interface(Error,
	errvt fn(Set)(std_Error* err, const char* errname, const char funcname[]);
      	noFail fn(Clear)();
      	errvt fn(Try)(errvt* errors_to_catch, len_t num);
      	noFail fn(Throw)();
	std_Error* fn(Get)();
)

#undef package




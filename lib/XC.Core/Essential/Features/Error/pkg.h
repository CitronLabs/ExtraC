#pragma once
#define __XC_FEATURE__
#include "../pkg.h"

#include "../Type/pkg.h"

#include "__macro_utils.h"

#define package std

#define OK std.Error.Code.NONE

#define ERR(code, msg) std.Error.Set(&(std_Error){code, msg}, #code, __func__, __module__)

#define check(...) for(std_Error* err = std.Error.Get(); err->errorcode != std.Error.Code.NONE; std.Error.Clear()) 	\
		   loop(i, 										\
	 		(sizeof((errvt[]){__VA_ARGS__}) / sizeof(errvt)) ? 				\
	 		(sizeof((errvt[]){__VA_ARGS__}) / sizeof(errvt)) : 1				\
		   ) if ((sizeof((errvt[]){__VA_ARGS__}) / sizeof(errvt)) ? 				\
    			 ((errvt[]){__VA_ARGS__})[i] == err->errorcode : err->errorcode != std.Error.Code.NONE)

#define try(...) std.Error.Clear(); 									\
		if(!std.Error.Try((errvt[]){__VA_ARGS__}, sizeof((errvt[]){__VA_ARGS__})/sizeof(errvt)))	\

#define catch else for(std_Error* err = std.Error.Get(); err->errorcode != std.Error.Code.NONE; std.Error.Clear())

#define throw(code, msg) ERR(code, msg); std.Error.Throw();
#define nullerr(var) ERR(std.Error.Code.NIL, #var " is null")


#define nonull(...)								\
    for(errvt __i = sizeof((const void*[]){__VA_ARGS__}), err = OK; __i; __i--)	\
        if(!err){								\
            if(((const void*[]){__VA_ARGS__})[__i - 1]) { continue; }		\
            else {								\
                err = ERR(std.Error.Code.NIL, "null value detected"); 		\
                println("NULL VALUE: ",  					\
			((char*[]){QUOTE_LIST(__VA_ARGS__)}[__i]));		\
            }									\
	} else

    



#define iferr(errorable) for(errvt err = (errorable); err; (std.Error.Clear(), err = OK))
#define NOT_IMPLEM(returnval) ERR(std.Error.Code.NOTIMPLEM, "not implemented yet..."); return returnval;

#define errnm  (std.Error.Get()->errorcode)
#define errstr (std.Error.Get()->message)

type(Error,
	errvt errorcode; 
	char* message;
);

Interface(Error,
	submodule(Code,
	    const errvt 
	    	NONE, NIL, FAIL, INVALID, INIT, NOTIMPLEM, BUSY;

	    values(DATA, errvt,			
	    	MEMALLOC, SIZETOOLARGE,		
	    	OUTOFRANGE, LIMIT, EMPTY, 	
	    	DSN				
	    )					
	    values(MEM, errvt,			
	    	OVERFLOW			
	    )		
	    values(STRING, errvt,			
	    	ENCODING			
	    )		
	)

	errvt fn(Set)(std_Error* err, const char* errname, const char funcname[], const char modulename[]);
      	noFail fn(Clear)();
      	errvt fn(Try)(errvt* errors_to_catch, len_t num);
      	noFail fn(Throw)();
	std_Error* fn(Get)();
)

#undef package




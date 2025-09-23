#pragma once
#include "pkg.h"

#if __Debug
// utility for quickly removing debug statements
      #define debug(...) __VA_ARGS__
#else
      #define debug(...) 
#endif

#define OK ERR_NONE

#define ERR(code, msg) Error.Set(&(Error_Instance){NULL,&Error,code, #code}, msg, __func__)

#define check(...) for(inst(Error) err = geterr(); err->errorcode != ERR_NONE; Error.Clear()) 	\
		   loop(i, 									\
	 		(sizeof((errvt[]){__VA_ARGS__}) / sizeof(errvt)) ? 			\
	 		(sizeof((errvt[]){__VA_ARGS__}) / sizeof(errvt)) : 1			\
		   ) if ((sizeof((errvt[]){__VA_ARGS__}) / sizeof(errvt)) ? 			\
    			 ((errvt[]){__VA_ARGS__})[i] == err->errorcode : err->errorcode != ERR_NONE)

#define try(...) Error.Clear(); 									\
		if(!Error.Try((errvt[]){__VA_ARGS__}, sizeof((errvt[]){__VA_ARGS__})/sizeof(errvt)))	\

#define catch else for(inst(Error) err = geterr(); err->errorcode != ERR_NONE; Error.Clear())

#define throw(code, msg) ERR(code, msg); Error.Throw();
#define nullerr(var) ERR(ERR_NULLPTR, #var " is null")
#define nonull(var, ...) if(var == NULL){errvt err = nullerr(var); __VA_ARGS__;}
#define iferr(errorable) for(errvt err = (errorable); err; Error.Clear())
#define NOT_IMPLEM(returnval) ERR(ERR_NOTIMPLEM, "not implemented yet..."); return returnval;
#define onFail bool __FuncFail = false; if(false){safe_fail: __FuncFail = true;} if(__FuncFail)
#define FAIL(code, msg) ERR(code, msg); goto safe_fail;

#define errnm  (geterr()->errorcode)
#define errstr (geterr()->message)

#define quiet() Error.Hide(); for(int i = 1; i--; Error.Show())

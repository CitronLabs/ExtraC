#pragma once
#define __XC_ENV_LIB_C__
#include "../pkg.h"

Interface(C_STR_TYPE,
	int 	  fn(isalnum)(int c);
	int 	  fn(isalpha)(int c);
	int 	  fn(isblank)(int c);
	int 	  fn(iscntrl)(int c);
	int 	  fn(isdigit)(int c);
	int 	  fn(isgraph)(int c);
	int 	  fn(islower)(int c);
	int 	  fn(isprint)(int c);
	int 	  fn(ispunct)(int c);
	int 	  fn(isspace)(int c);
	int 	  fn(isupper)(int c);
	int 	  fn(isxdigit)(int c);
	
	int 	  fn(tolower)(int c);
	int 	  fn(toupper)(int c);
)

#ifndef __XC_LIBC_HIDEMACROS

	#define isalnum(c)	      C.str.base.type.isalnum(c)	   
	#define isalpha(c)            C.str.base.type.isalpha(c)       
	#define isblank(c)            C.str.base.type.isblank(c)       
	#define iscntrl(c)            C.str.base.type.iscntrl(c)       
	#define isdigit(c)            C.str.base.type.isdigit(c)       
	#define isgraph(c)            C.str.base.type.isgraph(c)       
	#define islower(c)            C.str.base.type.islower(c)       
	#define isprint(c)            C.str.base.type.isprint(c)       
	#define ispunct(c)            C.str.base.type.ispunct(c)       
	#define isspace(c)            C.str.base.type.isspace(c)       
	#define isupper(c)            C.str.base.type.isupper(c)       
	#define isxdigit(c)           C.str.base.type.isxdigit(c)      
	                                                              
	#define tolower(c)            C.str.base.type.tolower(c)       
	#define toupper(c)            C.str.base.type.toupper(c)       

#endif                                                           

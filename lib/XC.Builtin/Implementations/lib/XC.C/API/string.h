#pragma once
#define __XC_ENV_LIB_C__
#include "../pkg.h"

#include "ctype.h"

#ifndef __XC_LIBC_HIDETYPES__

typedef __XC_UINT64_TYPE size_t;

#endif


Interface(C_STR,
  interface(C_STR_TYPE) type;
  namespace(mem,
	void* 	fn(memchr)(const void* str, int c, size_t n);	
	int 	fn(memcmp)(const void* str1, const void* str2, size_t n);	
	void*	fn(memcpy)(void* dest, const void* src, size_t n);	
	void*	fn(memmove)(void* dest, const void* src, size_t n);	
	void*	fn(memset)(void* str, int c, size_t n);  
  )  
  namespace(ops,
	char*	fn(strcat)(char* dest, const char* src);	     	
	char*	fn(strncat)(char* dest, const char* src, size_t n);	     	
	char*	fn(strchr)(const char* str, int c);	
	int 	fn(strcmp)(const char* str1, const char* str2);	
	int 	fn(strncmp)(const char* str1, const char* str2, size_t n);	
	int 	fn(strcoll)(const char* str1, const char* str2);	
	char*	fn(strcpy)(char* dest, const char* src);	
	char*	fn(strncpy)(char* dest, const char* src, size_t n);	
	size_t 	fn(strcspn)(const char* str1, const char* str2);	
	char*	fn(strerror)(int errnum);	
	size_t 	fn(strlen)(const char* str);	
	size_t 	fn(strnlen)(const char* str, size_t n);	
	char*	fn(strpbrk)(const char* str1, const char* str2);	
	char*	fn(strrchr)(const char* str, int c);	
	size_t 	fn(strspn)(const char* str1, const char* str2);	
	char*	fn(strstr)(const char* haystack, const char* needle);	
	char*	fn(strtok)(char* str, const char* delim);	
	size_t 	fn(strxfrm)(char* dest, const char* src, size_t n);  
  )
  namespace(utils,
	int 		fn(atoi)	(const char *str);
	long int 	fn(atol)	(const char *str);
	double 		fn(strtod)	(const char *str, char **endptr);
	long int 	fn(strtol)	(const char *str, char **endptr, int base);
	unsigned long 	fn(strtoul)	(const char *str, char **endptr, int base);

  )
);


#ifndef __XC_LIBC_HIDEMACROS__

#define memchr(_str, c, n)		C.str.base.mem.memchr(_str, c, n)		
#define memcmp(str1, str2, n)		C.str.base.mem.memcmp(str1, str2, n)		
#define memcpy(dest, src, n)		C.str.base.mem.memcpy(dest, src, n)		
#define memmove(dest, src, n)		C.str.base.mem.memmove(dest, src, n)		
#define memset(_str, c, n)		C.str.base.mem.memset(_str, c, n)	  
                                                                               
#define strcat(dest, src)		C.str.base.ops.strcat(dest, src)		     	
#define strncat(dest, src, n)		C.str.base.ops.strncat(dest, src, n)		     	
#define strchr(_str, c)			C.str.base.ops.strchr(_str, c)			
#define strcmp(str1, str2)		C.str.base.ops.strcmp(str1, str2)		
#define strncmp(str1, str2, n)		C.str.base.ops.strncmp(str1, str2, n)		
#define strcoll(str1, str2)		C.str.base.ops.strcoll(str1, str2)		
#define strcpy(dest, src)		C.str.base.ops.strcpy(dest, src)		
#define strncpy(dest, src, n)		C.str.base.ops.strncpy(dest, src, n)		
#define strcspn(str1, str2)		C.str.base.ops.strcspn(str1, str2)		
#define strerror(errnum)		C.str.base.ops.strerror(errnum)		
#define strlen(_str)			C.str.base.ops.strlen(_str)			
#define strnlen(_str, n)		C.str.base.ops.strnlen(_str, n)			
#define strpbrk(str1, str2)		C.str.base.ops.strpbrk(str1, str2)		
#define strrchr(_str, c)		C.str.base.ops.strrchr(_str, c)			
#define strspn(str1, str2)		C.str.base.ops.strspn(str1, str2)		
#define strstr(haystack, needle)	C.str.base.ops.strstr(haystack, needle)	
#define strtok(_str, delim)		C.str.base.ops.strtok(_str, delim)		
#define strxfrm(dest, src, n)		C.str.base.ops.strxfrm(dest, src, n)	  

#define atoi(_str)			C.str.base.utils.atoi(_str)
#define atol(_str)                      C.str.base.utils.atol(_str)
#define strtod(_str, endptr)            C.str.base.utils.strtod(_str, endptr)
#define strtol(_str, endptr, _base)     C.str._base.utils.strtol(_str, endptr, base)
#define strtoul(_str, endptr, _base)    C.str._base.utils.strtoul(_str, endptr, base)

#endif

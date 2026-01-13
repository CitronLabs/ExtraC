#pragma once
#define __XC_ENV_LIB_C__
#include "../pkg.h"

#include "wctype.h"
#include "stdarg.h"

#ifndef __XC_LIBC_HIDETYPES__

typedef __XC_UINT64_TYPE size_t;

typedef __XC_INT32_TYPE char32_t;
typedef __XC_INT16_TYPE char16_t;
typedef char32_t wchar_t;
typedef __XC_UINT32_TYPE wint_t;

typedef struct FILE FILE;
typedef struct __mbstate_t mbstate_t;

struct tm;

#endif

Interface(C_WSTR,
  interface(C_WSTR_TYPE) type;
  namespace(ops,
	wchar_t* fn(wcscat)(wchar_t *s1, const wchar_t *s2);
	wchar_t* fn(wcschr)(const wchar_t *s, wchar_t c);
	int 	 fn(wcscmp)(const wchar_t *s1, const wchar_t *s2);
	int 	 fn(wcscoll)(const wchar_t *s1, const wchar_t *s2);
	wchar_t* fn(wcscpy)(wchar_t *s1, const wchar_t *s2);
	size_t 	 fn(wcscspn)(const wchar_t *s1, const wchar_t *s2);
	size_t 	 fn(wcslen)(const wchar_t *s);
	wchar_t* fn(wcsncat)(wchar_t *s1, const wchar_t *s2, size_t n);
	int 	 fn(wcsncmp)(const wchar_t *s1, const wchar_t *s2, size_t n);
	wchar_t* fn(wcsncpy)(wchar_t *s1, const wchar_t *s2, size_t n);
	wchar_t* fn(wcspbrk)(const wchar_t *s1, const wchar_t *s2);
	wchar_t* fn(wcsrchr)(const wchar_t *s, wchar_t c);
	size_t   fn(wcsspn)(const wchar_t *s1, const wchar_t *s2);
	wchar_t* fn(wcsstr)(const wchar_t *s1, const wchar_t *s2);
	wchar_t* fn(wcstok)(wchar_t *s1, const wchar_t *s2, wchar_t **ptr);
	size_t   fn(wcsxfrm)(wchar_t *s1, const wchar_t *s2, size_t n);
  )
  namespace(mem,
	wchar_t* fn(wmemchr)(const wchar_t *s, wchar_t c, size_t n);
	int      fn(wmemcmp)(const wchar_t *s1, const wchar_t *s2, size_t n);
	wchar_t* fn(wmemcpy)(wchar_t *s1, const wchar_t *s2, size_t n);
	wchar_t* fn(wmemmove)(wchar_t *s1, const wchar_t *s2, size_t n);
	wchar_t* fn(wmemset)(wchar_t *s, wchar_t c, size_t n);
  )
  namespace(utils,
	int 	 fn(mblen)(const char *str, size_t n);
	size_t 	 fn(mbstowcs)(wchar_t *pwcs, const char *str, size_t n);
	int 	 fn(mbtowc)(wchar_t *pwc, const char *str, size_t n);
	size_t 	 fn(wcstombs)(char *str, const wchar_t *pwcs, size_t n);
	int 	 fn(wctomb)(char *str, wchar_t wchar);
	wint_t 	 fn(btowc)(int c);
	size_t 	 fn(mbrlen)(const char *s, size_t n, mbstate_t *ps);
	size_t 	 fn(mbrtowc)(wchar_t *pwc, const char *s, size_t n, mbstate_t *ps);
	int 	 fn(mbsinit)(const mbstate_t *ps);
	size_t 	 fn(mbsrtowcs)(wchar_t *dst, const char **src, size_t len, mbstate_t *ps);
	size_t 	 fn(wcrtomb)(char *s, wchar_t wc, mbstate_t *ps);
	size_t 	 fn(wcsrtombs)(char *dst, const wchar_t **src, size_t len, mbstate_t *ps);
	int 	 fn(fwide)(FILE *stream, int mode);
	double 	 fn(wcstod)(const wchar_t *nptr, wchar_t **endptr);
	float 	 fn(wcstof)(const wchar_t *nptr, wchar_t **endptr);
	long 	 fn(wcstol)(const wchar_t *nptr, wchar_t **endptr, int base);
	int 	 fn(wctob)(wint_t c);
	long double 	   fn(wcstold)(const wchar_t *nptr, wchar_t **endptr);
	long long 	   fn(wcstoll)(const wchar_t *nptr, wchar_t **endptr, int base);
	unsigned long int  fn(wcstoul)(const wchar_t *nptr, wchar_t **endptr, int base);
	unsigned long long fn(wcstoull)(const wchar_t *nptr, wchar_t **endptr, int base);
  )
)

#ifndef __XC_LIBC_HIDEMACROS__

#define wcscat(s1, s2)			C.str.wide.ops.wcscat(s1, s2)		    
#define wcschr(s, c)			C.str.wide.ops.wcschr(s, c)		    
#define wcscmp(s1, s2)			C.str.wide.ops.wcscmp(s1, s2)		    
#define wcscoll(s1, s2)			C.str.wide.ops.wcscoll(s1, s2)		    
#define wcscpy(s1, s2)			C.str.wide.ops.wcscpy(s1, s2)		    
#define wcscspn(s1, s2)			C.str.wide.ops.wcscspn(s1, s2)		    
#define wcslen( s)			C.str.wide.ops.wcslen( s)		    
#define wcsncat(s1, s2, n)		C.str.wide.ops.wcsncat(s1, s2, n)	    
#define wcsncmp(s1, s2, n)		C.str.wide.ops.wcsncmp(s1, s2, n)	    
#define wcsncpy(s1, s2, n)		C.str.wide.ops.wcsncpy(s1, s2, n)	    
#define wcspbrk(s1, s2)			C.str.wide.ops.wcspbrk(s1, s2)		    
#define wcsrchr(s, c)			C.str.wide.ops.wcsrchr(s, c)		    
#define wcsspn(s1, s2)			C.str.wide.ops.wcsspn(s1, s2)		    
#define wcsstr(s1, s2)			C.str.wide.ops.wcsstr(s1, s2)		    
#define wcstok(s1, s2, ptr)		C.str.wide.ops.wcstok(s1, s2, ptr)	    
#define wcsxfrm(s1, s2, n)		C.str.wide.ops.wcsxfrm(s1, s2, n)	    
                                                                            
#define wmemchr( s, c, n)		C.str.wide.mem.wmemchr( s, c, n)	    
#define wmemcmp( s1,  s2, n)		C.str.wide.mem.wmemcmp( s1,  s2, n)	    
#define wmemcpy(s1,  s2, n)		C.str.wide.mem.wmemcpy(s1,  s2, n)	    
#define wmemmove(s1,  s2, n)		C.str.wide.mem.wmemmove(s1,  s2, n)	    
#define wmemset(s, c, n)		C.str.wide.mem.wmemset(s, c, n)	    
                                                                              
#define mblen(str, n)                   C.lib.wide.utils.mblen(str, n)
#define mbstowcs(pwcs, str, n)          C.lib.wide.utils.mbstowcs(pwcs, str, n)
#define mbtowc(pwc, str, n)             C.lib.wide.utils.mbtowc(pwc, str, n)
#define wcstombs(str, pwcs, n)          C.lib.wide.utils.wcstombs(str, pwcs, n)
#define wctomb(str, wchar)              C.lib.wide.utils.wctomb(str, wchar)
#define btowc(c)			C.str.wide.utils.btowc(c)		    
#define mbrlen(s, n, ps)		C.str.wide.utils.mbrlen(s, n, ps)	    
#define mbrtowc(pwc,  s, n, ps)		C.str.wide.utils.mbrtowc(pwc,  s, n, ps)	    
#define mbsinit(ps)			C.str.wide.utils.mbsinit(ps)		    
#define mbsrtowcs(dst, src, len, ps)	C.str.wide.utils.mbsrtowcs(dst, src, len, ps)
#define wcrtomb(s, wc, ps)		C.str.wide.utils.wcrtomb(s, wc, ps)	    
#define wcsrtombs(dst, src, len, ps)	C.str.wide.utils.wcsrtombs(dst, src, len, ps)
#define fwide(stream, mode)		C.str.wide.utils.fwide(stream, mode)	    
#define wcstod(nptr, endptr)		C.str.wide.utils.wcstod(nptr, endptr)	    
#define wcstof(nptr, endptr)           	C.str.wide.utils.wcstof(nptr, endptr)        
#define wcstol(nptr, endptr, base)     	C.str.wide.utils.wcstol(nptr, endptr, base)  
#define wcstold(nptr, endptr)           C.str.wide.utils.wcstold(nptr, endptr)       
#define wctob(c)                        C.str.wide.utils.wctob(c)                    
#define wcstoll(nptr, endptr, base)     C.str.wide.utils.wcstoll(nptr, endptr, base) 
#define wcstoul(nptr, endptr, base)     C.str.wide.utils.wcstoul(nptr, endptr, base) 
#define wcstoull(nptr, endptr, base)    C.str.wide.utils.wcstoull(nptr, endptr, base)

#endif

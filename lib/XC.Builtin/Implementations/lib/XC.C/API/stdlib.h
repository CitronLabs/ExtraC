#pragma once
#define __XC_ENV_LIB_C__
#include "../pkg.h"

#include "stddef.h"
#include "wchar.h"

#ifndef __XC_LIBC_HIDETYPES__

	typedef struct { int quot; int rem; } div_t;

	typedef struct { long quot; long rem; } ldiv_t;

#endif

Interface(C_GEN,
  namespace(math,
	int 		fn(abs)		(int x);
	div_t 		fn(div)		(int numer, int denom);
	long int 	fn(labs)	(long int x);
	ldiv_t 		fn(ldiv)	(long int numer, long int denom);
	int 		fn(rand)	(void);
	void 		fn(srand)	(unsigned int seed);
  )
  namespace(utils,
	void 		fn(abort)	(void);
	int 		fn(atexit)	(void(*func)(void));
	void		fn(exit)	(int status);
	char* 		fn(getenv)	(const char *name);
	int 		fn(system)	(const char *string);
	void* 		fn(bsearch)	(const void *key, const void *base, size_t nitems, size_t size, int(*compar)(const void *, const void *));
	void 		fn(qsort)	(void *base, size_t nitems, size_t size, int(*compar)(const void *, const void*));
  )
  namespace(alloc,
	void* 		fn(calloc)	(size_t nitems, size_t size);
	void 		fn(free)	(void *ptr);
	void* 		fn(malloc)	(size_t size);
	void* 		fn(alloca)	(size_t size);
	void* 		fn(realloc)	(void *ptr, size_t size);
  )
)

#ifndef __XC_LIBC_HIDEMACROS__

#define EXIT_FAILURE -1
#define EXIT_SUCCESS 0

#define abs(x)                                          C.lib.math.abs(x)
#define div(numer, denom)                               C.lib.math.div(numer, denom)
#define labs(x)                                         C.lib.math.labs(x)
#define ldiv(numer, denom)                              C.lib.math.ldiv(numer, denom)
#define rand(void)                                      C.lib.math.rand(void)
#define srand(seed)                                     C.lib.math.srand(seed)
#define abort(void)                                     C.lib.utils.abort(void)
#define atexit(func)                                    C.lib.utils.atexit(func)
#define exit(status)                                    C.lib.utils.exit(status)
#define getenv(name)                                    C.lib.utils.getenv(name)
#define system(string)                                  C.lib.utils.system(string)
#define bsearch(key, base, nitems, size, compar)        C.lib.utils.bsearch(key, base, nitems, size, compar)
#define qsort(base, nitems, size, compar)               C.lib.utils.qsort(base, nitems, size, compar)
#define calloc(nitems, size)                            C.lib.alloc.calloc(nitems, size)
#define free(ptr)                                       C.lib.alloc.free(ptr)
#define malloc(size)                                    C.lib.alloc.malloc(size)
#define alloca(size)                                    C.lib.alloc.alloca(size)
#define realloc(ptr, size)                              C.lib.alloc.realloc(ptr, size)

#endif









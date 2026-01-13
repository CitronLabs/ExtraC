#pragma once
#define __XC_ENV_LIB_C__
#include "../pkg.h"


#ifndef __XC_LIBC_HIDETYPES__

	typedef __XC_UINT64_TYPE size_t;

	#if defined(__x86_64__) || defined(__aarch64__) || defined(_LP64)
		typedef long ptrdiff_t;
	#else
    		typedef int ptrdiff_t;
	#endif

	typedef __XC_RUNE_TYPE wchar_t;


#endif

#ifndef __XC_LIBC_HIDEMACROS__

	#define null ((void*)0)

	#define offsetof(type, member) ((size_t)&(((type *)0)->member))

#endif


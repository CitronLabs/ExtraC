#pragma once
#include "stddef.h"

#ifndef __XC_LIBC_HIDEMACROS

	// Define the abort function declaration, required by the assert macro
	// This function will rely on your XC.Sys.terminate implementation
	void __xc_assert_fail(const char *expression, const char *file, unsigned int line, const char *function);


	#ifdef NDEBUG
	    // If NDEBUG is defined, the assert macro is disabled (C11 requirement)
	    #define assert(expression) ((void)0)
	#else
	    // If NDEBUG is NOT defined, the assert macro performs its check
	    #define assert(expression) \
	        ((expression) ? (void)0 : __xc_assert_fail(#expression, __FILE__, __LINE__, __func__))
	#endif

#endif

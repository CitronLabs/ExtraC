#pragma once

#define __XC_ENV_LIB_C__
#include "../pkg.h"
#include "stddef.h" 


#ifndef __XC_LIBC_HIDETYPES__

	// jmp_buf must be an array type capable of holding the execution environment 
	// (registers, stack pointer, frame pointer, etc.)
	// The size and contents are compiler and architecture dependent.

	#if defined(__x86_64__)
	    // Placeholder for x86_64: usually requires saving ~16 registers plus stack/frame pointers.
	    typedef long long jmp_buf[16]; 
	#elif defined(__aarch64__)
	    // Placeholder for AArch64: requires saving general purpose and possibly floating point registers.
	    typedef long long jmp_buf[32];
	#else
	    // Generic placeholder for portability. Actual size is defined by the context-CRT.
	    typedef int jmp_buf[1]; 
#endif

#endif

Interface(C_SETJMP,
	int fn(setjmp)(jmp_buf env);
	void fn(longjmp)(jmp_buf env, int val);
)

#ifndef __XC_LIBC_HIDEMACROS__


#define setjmp(env)		C.jmp.setjmp(env)
#define longjmp(env, val)	C.jmp.longjmp(env, val)


#endif

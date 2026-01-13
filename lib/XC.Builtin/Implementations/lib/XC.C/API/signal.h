#pragma once

#define __XC_ENV_LIB_C__
#include "../pkg.h"
#include "stddef.h" 


#ifndef __XC_LIBC_HIDETYPES__

	typedef int sig_atomic_t; // Integer type that can be accessed atomically (thread/interrupt safe)

	// The type for a signal handler function
	typedef void (*__sighandler_t)(int);

#endif

Interface(C_SIGNAL,
	// Sets the action for the given signal
	__sighandler_t fn(signal)(int sig, __sighandler_t func);

	// Sends a signal to the executing program (relies on XC.Sys or internal mechanism)
	int fn(raise)(int sig);

)

#ifndef __XC_LIBC_HIDEMACROS__

	// Signal actions
	#define SIG_DFL     ((__sighandler_t)0)  // Default signal handling
	#define SIG_IGN     ((__sighandler_t)1)  // Ignore signal
	#define SIG_ERR     ((__sighandler_t)-1) // Error return value

	// Standard Signals (Minimal set for compliance)
	#define SIGABRT     1 // Abort program (e.g., from assert)
	#define SIGFPE      2 // Floating-point exception (e.g., division by zero)
	#define SIGILL      3 // Illegal instruction
	#define SIGINT      4 // Interrupt (e.g., Ctrl+C)
	#define SIGSEGV     5 // Segmentation violation (illegal memory access)
	#define SIGTERM     6 // Termination request


	#define signal(_sig, func) C.sig.signal(_sig, func)
	#define raise(_sig)	   C.sig.raise(_sig)


#endif 


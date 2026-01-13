#pragma once


#ifndef __XC_LIBC_HIDEMACROS

	#define EDOM    1 // Mathematics argument out of domain (e.g., sqrt(-1))
	#define ERANGE  2 // Result too large (range error)
	#define EILSEQ  3 // Illegal byte sequence (e.g., in multibyte conversion)

	#ifdef __GNUC__
	    // GCC/Clang specific: A common way to implement a thread-local errno.
	    extern int *__errno_location(void);
	    #define errno (*__errno_location())
	#elif defined(_MSC_VER)
	    // MSVC specific: Another common way to access the thread-local errno.
	    extern int *__cmost_errno(void);
	    #define errno (*__cmost_errno())
	#else
	    // Generic placeholder: If the environment doesn't support thread-local storage,
	    // this defines a global errno (NON-THREAD-SAFE).
	    extern int errno; 
	#endif

#endif

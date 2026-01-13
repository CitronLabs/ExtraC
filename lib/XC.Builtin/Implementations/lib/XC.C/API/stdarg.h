#pragma once

#ifndef __XC_LIBC_HIDETYPES__
	#if defined(__GNUC__) || defined(__clang__)
	    // Use the compiler's built-in va_list definition
	    typedef __builtin_va_list va_list;
	#else
	    // Generic placeholder if no built-in type is available.
	    // Must be large enough to hold the necessary register/stack frame info.
	    typedef void *va_list[1]; 
	#endif
#endif

#ifndef __XC_LIBC_HIDEMACROS__

	// Initializes 'ap' to point to the first variable argument.
	// 'parmN' is the last named parameter before the ellipsis (...).
	#if defined(__GNUC__) || defined(__clang__)
	    #define va_start(ap, parmN)   __builtin_va_start(ap, parmN)
	#else
	    // Generic implementation requires knowing stack layout, making it non-portable.
	    // A simplified, conceptual view:
	    // #define va_start(ap, parmN)  ((ap) = (va_list)((char *)&parmN + sizeof(parmN)))
	    // NOTE: Actual implementation requires careful alignment and calling convention knowledge.
	    // For a real XC runtime, this MUST use the target compiler's intrinsic.
	    #error "Compiler specific va_start intrinsic not defined for this target."
	#endif

	// Retrieves the next argument in the list as the specified type 'type'.
	#if defined(__GNUC__) || defined(__clang__)
    		#define va_arg(ap, type)      __builtin_va_arg(ap, type)
	#else
    	// Conceptual view:
    	// #define va_arg(ap, type) \
	    //     (*(type *)((ap) = (va_list)((char *)(ap) + sizeof(type)), \
	    //                 (char *)(ap) - sizeof(type)))
	    #error "Compiler specific va_arg intrinsic not defined for this target."
	#endif

	// Cleans up the argument list.
	#if defined(__GNUC__) || defined(__clang__)
	    #define va_end(ap)            __builtin_va_end(ap)
	#else
	    #define va_end(ap)            ((void)0)
	#endif

	// C99: Allows copying a va_list state.
	#define va_copy(dest, src)  __builtin_va_copy(dest, src)

#endif 

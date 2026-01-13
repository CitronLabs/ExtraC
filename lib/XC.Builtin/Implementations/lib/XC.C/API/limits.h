#pragma once
#include "../../XC/config.c"

#ifndef __XC_LIBC_HIDEMACROS__

	
	// Assume 8-bit characters (common in all modern systems)
	#define CHAR_BIT    8
	
	#define SCHAR_MIN   (-128)
	#define SCHAR_MAX   (127)
	#define UCHAR_MAX   (255)
	
	// The character type limits depend on whether 'char' is signed or unsigned by default
	#if ('\x80' < 0)
	    #define CHAR_MIN    SCHAR_MIN
	    #define CHAR_MAX    SCHAR_MAX
	#else
	    #define CHAR_MIN    0
	    #define CHAR_MAX    UCHAR_MAX
	#endif
	
	
	// Short limits (at least 16 bits)
	#define SHRT_MIN    (-32767 - 1)
	#define SHRT_MAX    (32767)
	#define USHRT_MAX   (65535)
	
	// Int limits (at least 16 bits, typically 32 bits or more)
	#define INT_MIN     (-2147483647 - 1)
	#define INT_MAX     (2147483647)
	#define UINT_MAX    (4294967295U)
	
	// Long limits (at least 32 bits, typically 64 bits on modern 64-bit systems)
	// Assuming 64-bit long for robustness on 64-bit platforms
	#if __WORDSIZE == 64
	    #define LONG_MIN    (-9223372036854775807L - 1L)
	    #define LONG_MAX    (9223372036854775807L)
	    #define ULONG_MAX   (18446744073709551615UL)
	#else
	    #define LONG_MIN    (-2147483647L - 1L)
	    #define LONG_MAX    (2147483647L)
	    #define ULONG_MAX   (4294967295UL)
	#endif
	
	// Long long limits (at least 64 bits)
	#define LLONG_MIN   (-9223372036854775807LL - 1LL)
	#define LLONG_MAX   (9223372036854775807LL)
	#define ULLONG_MAX  (18446744073709551615ULL)
	
	// Size of the largest object that can be stored in a region of memory
	// (Relies on the type of size_t, typically unsigned long or unsigned int)
	#define SSIZE_MAX   LONG_MAX // Used for ssize_t (signed size_t equivalent)

#endif // XC_LIMITS_H

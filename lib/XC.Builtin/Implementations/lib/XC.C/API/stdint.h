#pragma once
#define __XC_ENV_LIB_C__

#ifndef __XC_LIBC_HIDETYPES__

	// Exact width integer types
	typedef signed char         int8_t;
	typedef short               int16_t;
	typedef int                 int32_t;
	typedef long long           int64_t;
	
	typedef unsigned char       uint8_t;
	typedef unsigned short      uint16_t;
	typedef unsigned int        uint32_t;
	typedef unsigned long long  uint64_t;
	
	
	typedef int8_t              int_least8_t;
	typedef int16_t             int_least16_t;
	typedef int32_t             int_least32_t;
	typedef int64_t             int_least64_t;
	
	typedef uint8_t             uint_least8_t;
	typedef uint16_t            uint_least16_t;
	typedef uint32_t            uint_least32_t;
	typedef uint64_t            uint_least64_t;
	
	typedef int                 int_fast8_t;
	typedef int                 int_fast16_t;
	typedef int                 int_fast32_t;
	typedef long long           int_fast64_t;
	
	typedef unsigned int        uint_fast8_t;
	typedef unsigned int        uint_fast16_t;
	typedef unsigned int        uint_fast32_t;
	typedef unsigned long long  uint_fast64_t;
	
	
	// The definition of intptr_t and uintptr_t is architecture-dependent.
	// We use compiler intrinsics for maximum compatibility, assuming a 32-bit or 64-bit architecture.
	#if defined(_LP64) || defined(__x86_64__) || defined(__aarch64__)
	    typedef long int            intptr_t;
	    typedef unsigned long int   uintptr_t;
	#else
	    typedef int                 intptr_t;
	    typedef unsigned int        uintptr_t;
	#endif
	
	typedef long long           intmax_t;
	typedef unsigned long long  uintmax_t;
	

#endif // HIDE_TYPES

#ifndef __XC_LIBC_HIDEMACROS__

	#define INT8_MIN    (-128)
	#define INT16_MIN   (-32767 - 1)
	#define INT32_MIN   (-2147483647 - 1)
	#define INT64_MIN   (-9223372036854775807LL - 1LL)
	
	#define INT8_MAX    (127)
	#define INT16_MAX   (32767)
	#define INT32_MAX   (2147483647)
	#define INT64_MAX   (9223372036854775807LL)
	
	#define UINT8_MAX   (255U)
	#define UINT16_MAX  (65535U)
	#define UINT32_MAX  (4294967295U)
	#define UINT64_MAX  (18446744073709551615ULL)
	
	// ... (similar definitions for _LEAST, _FAST, PTR, and MAX limits) ...
	
	// ----------------------------------------------------------------------
	// 7. Macros for Integer Constants
	// ----------------------------------------------------------------------
	
	#define INT8_C(x)       x
	#define INT16_C(x)      x
	#define INT32_C(x)      x
	#define INT64_C(x)      x ## LL
	
	#define UINT8_C(x)      x ## U
	#define UINT16_C(x)     x ## U
	#define UINT32_C(x)     x ## U
	#define UINT64_C(x)     x ## ULL
	
	#define INTMAX_C(x)     x ## LL
	#define UINTMAX_C(x)    x ## ULL

#endif //HIDE_MACROS








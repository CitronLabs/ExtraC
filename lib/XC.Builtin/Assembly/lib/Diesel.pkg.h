#pragma once
#include "Diesel.UAL/pkg.h"

#if defined(DIESEL_ARCH_X86)
	#if 	DIESEL_ARCH_X86 == 64
		#include "Diesel.x86/64/pkg.h"
	#elif 	DIESEL_ARCH_X86 == 32
		#include "Diesel.x86/32/pkg.h"
	#else 
	#error "Diesel.UAL: Unsupported System Architecture"
	#endif
#elif defined(DIESEL_ARCH_ARM)
	#if 	DIESEL_ARCH_ARM == 64
		#include "Diesel.ARM/64/pkg.h"
	#elif 	DIESEL_ARCH_ARM == 32
		#include "Diesel.ARM/32/pkg.h"
	#else 
	#error "Diesel.UAL: Unsupported System Architecture"
	#endif
#elif defined(DIESEL_ARCH_RISCV)

#else 
#error "Diesel.UAL: Unsupported System Architecture"
#endif

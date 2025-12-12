#pragma once
#define __UNDEFINED__  	static_assert(0, 					\
			"The current implementaion does not define this macro"	\
			);


#define asm __asm__ volatile

#define mov(dest, src) 		__UNDEFINED__
#define load(dest, src) 	__UNDEFINED__
#define store(dest, src) 	__UNDEFINED__

#define fn_start(name)

#define fn_end
#define fn_call(name)

#define lbl(name)
#define jmp(name)

#define add(lv, rv)		__UNDEFINED__

#define gpr_a 			__UNDEFINED__
#define gpr_b 			__UNDEFINED__
#define gpr_c 			__UNDEFINED__
#define gpr_d 			__UNDEFINED__




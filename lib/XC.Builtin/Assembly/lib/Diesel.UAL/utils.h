#define __UNDEFINED__  	static_assert(0, 					\
			"The current implementaion does not define this macro"	\
			);


#define asm __asm__ volatile
#define asmFn [[gnu::naked]] void

/* ual_arg(n): References the nth C-input operand (e.g., %0, %1) */
#define narg(n) "%" #n

/* ual_input_ref(var): Maps a C variable to a register for use in ual_asm */
#define input_ref(var) : : "r"(&(var))


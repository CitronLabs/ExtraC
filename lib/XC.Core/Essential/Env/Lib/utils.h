#include "__macro_utils.h"


#define lenof(...) ((len_t)(sizeof(__VA_ARGS__) / sizeof(*__VA_ARGS__)))
#define elif(...) else if(__VA_ARGS__)


#define pntr_shift(ptr, shift_amt) ptr = ((pntr)(&(((u8*)ptr)[shift_amt])))
#define pntr_shiftcpy(ptr, shift_amt) ((pntr)(&(((u8*)ptr)[shift_amt])))
#define pntr_asVal(addr) (*(pntrval*)&addr)
#define pntr_dist(a, b) (pntr_asVal(b) - pntr_asVal(a))

#define usign_max(type) (~(type){0})   
#define sign_max(type)  ((~(type){0})>>1)
#define double_max ((double)1.7976931348623157e+308)
#define float_max ((float)3.40282347e+38F)

#define usign_min(type) ((type){0})	
#define sign_min(type) ~(sign_max(type))	
#define double_min ((double)2.2250738585072014e-308)	
#define float_min ((float)1.17549435e-38F)

#define maxof(type) 		\
_Generic(((type){0}), 		\
i32:    sign_max(i32), 		\
u32:    usign_max(u32),		\
i64:    sign_max(i64),  	\
u64:    usign_max(u64), 	\
i16:    sign_max(i16), 		\
u16:    usign_max(u16), 	\
i8:     sign_max(i8),		\
u8:     usign_max(u8),		\
float:  float_max, 		\
double: double_max,		\
void*:  ~pntr_asVal(((type){0}))\
)

#define minof(type) 	\
_Generic(((type){0}),  	\
i32:    sign_min(i32), 	\
u32:    usign_min(u32),	\
i64:    sign_min(i64),  \
u64:    usign_min(u64), \
i16:    sign_min(i16), 	\
u16:    usign_min(u16), \
i8:     sign_min(i8),	\
u8:     usign_min(u8),	\
float:  float_min, 	\
double: double_min,	\
void*:  ((pntr){0})	\
)


#define args(...) \
    (vargs){__VA_OPT__(__ARGS_sizeof_sum_helper(__VA_ARGS__))} \
    __VA_OPT__(, __VA_ARGS__)

#define __ vargs args, ...
#define varg(type) args.size ? (args.size -= sizeof(type), __XC_VARGS_GETARG(*args.data, type)) : (type){0}

#define va_use for(int __data_is_set = args.data ? 2 : 1; __data_is_set > 0;)		\
		for((args.data = !args.data ? &(__XC_VARGS_TYPE){0} : args.data), 	\
		    __XC_VARGS_START(*args.data, args); 				\
		    __data_is_set-- > 0; 						\
		    __XC_VARGS_END(*args.data), 					\
		    args.data = __data_is_set-- ? args.data : NULL)




// Helper macro to count the number of arguments (up to a certain limit)
#define __USE_GET_MACRO(	\
	_1, _2, _3, _4, 	\
	_5, _6, _7, _8, 	\
	_9, _10, _11, _12, 	\
	_13, _14, _15, _16, 	\
	_17, _18, _19, _20, 	\
	NAME, ...) 		\
	NAME



#define __USE_FOR_EACH_NARG(...)	\
	__USE_GET_MACRO(__VA_ARGS__, 	\
	     	__USE_FOR_EACH_20, 	\
	     	__USE_FOR_EACH_19, 	\
	     	__USE_FOR_EACH_18, 	\
		__USE_FOR_EACH_17, 	\
	   	__USE_FOR_EACH_16,	\
	   	__USE_FOR_EACH_15, 	\
	   	__USE_FOR_EACH_14, 	\
	     	__USE_FOR_EACH_13, 	\
		__USE_FOR_EACH_12, 	\
	   	__USE_FOR_EACH_11,	\
	   	__USE_FOR_EACH_10, 	\
	   	__USE_FOR_EACH_9, 	\
	     	__USE_FOR_EACH_8, 	\
		__USE_FOR_EACH_7, 	\
	   	__USE_FOR_EACH_6,	\
	   	__USE_FOR_EACH_5, 	\
	   	__USE_FOR_EACH_4, 	\
	   	__USE_FOR_EACH_3, 	\
	   	__USE_FOR_EACH_2, 	\
	   	__USE_FOR_EACH_1	\
	)

// Base case for a single argument
#define __USE_FOR_EACH_1(MACRO, package, arg) MACRO(package, arg)

// Recursive cases for multiple arguments
#define __USE_FOR_EACH_2(MACRO, package, arg, ...) MACRO(package, arg) __USE_FOR_EACH_1(MACRO, package, __VA_ARGS__)
#define __USE_FOR_EACH_3(MACRO, package, arg, ...) MACRO(package, arg) __USE_FOR_EACH_2(MACRO, package, __VA_ARGS__)
#define __USE_FOR_EACH_4(MACRO, package, arg, ...) MACRO(package, arg) __USE_FOR_EACH_3(MACRO, package, __VA_ARGS__)
#define __USE_FOR_EACH_5(MACRO, package, arg, ...) MACRO(package, arg) __USE_FOR_EACH_4(MACRO, package, __VA_ARGS__)
#define __USE_FOR_EACH_6(MACRO, package, arg, ...) MACRO(package, arg) __USE_FOR_EACH_5(MACRO, package, __VA_ARGS__)
#define __USE_FOR_EACH_7(MACRO, package, arg, ...) MACRO(package, arg) __USE_FOR_EACH_6(MACRO, package, __VA_ARGS__)
#define __USE_FOR_EACH_8(MACRO, package, arg, ...) MACRO(package, arg) __USE_FOR_EACH_7(MACRO, package, __VA_ARGS__)

#define __USE_FOR_EACH_9(MACRO, package, arg, ...) MACRO(package, arg) __USE_FOR_EACH_8(MACRO, package, __VA_ARGS__)
#define __USE_FOR_EACH_10(MACRO, package, arg, ...) MACRO(package, arg) __USE_FOR_EACH_9(MACRO, package, __VA_ARGS__)
#define __USE_FOR_EACH_11(MACRO, package, arg, ...) MACRO(package, arg) __USE_FOR_EACH_10(MACRO, package, __VA_ARGS__)
#define __USE_FOR_EACH_12(MACRO, package, arg, ...) MACRO(package, arg) __USE_FOR_EACH_11(MACRO, package, __VA_ARGS__)

#define __USE_FOR_EACH_13(MACRO, package, arg, ...) MACRO(package, arg) __USE_FOR_EACH_12(MACRO, package, __VA_ARGS__)
#define __USE_FOR_EACH_14(MACRO, package, arg, ...) MACRO(package, arg) __USE_FOR_EACH_13(MACRO, package, __VA_ARGS__)
#define __USE_FOR_EACH_15(MACRO, package, arg, ...) MACRO(package, arg) __USE_FOR_EACH_14(MACRO, package, __VA_ARGS__)
#define __USE_FOR_EACH_16(MACRO, package, arg, ...) MACRO(package, arg) __USE_FOR_EACH_15(MACRO, package, __VA_ARGS__)

#define __USE_FOR_EACH_17(MACRO, package, arg, ...) MACRO(package, arg) __USE_FOR_EACH_16(MACRO, package, __VA_ARGS__)
#define __USE_FOR_EACH_18(MACRO, package, arg, ...) MACRO(package, arg) __USE_FOR_EACH_17(MACRO, package, __VA_ARGS__)
#define __USE_FOR_EACH_19(MACRO, package, arg, ...) MACRO(package, arg) __USE_FOR_EACH_18(MACRO, package, __VA_ARGS__)
#define __USE_FOR_EACH_20(MACRO, package, arg, ...) MACRO(package, arg) __USE_FOR_EACH_19(MACRO, package, __VA_ARGS__)

// Main macro to initiate the loop
#define __USE_FOR_EACH(MACRO, package, ...) __USE_FOR_EACH_NARG(__VA_ARGS__)(MACRO, package, __VA_ARGS__)











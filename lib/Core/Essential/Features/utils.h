#pragma once
#define __XC_FEATURE__
#include "pkg.h"

#define Ops(type) 						\
	const struct type##_Ops_t type##_OpsImpl =

#define DO_LATER(...) static_assert(false , __VA_ARGS__);

#define getbitflagsinit len_t currentflag = 0, flagschecked = 0; 

#define getbitflags(flags, ...)											\
	currentflag = 0;											\
	flagschecked = 0;											\
	for (len_t bitmask = 1;(flags - flagschecked) > 0; bitmask*=2) 					\
	{													\
		currentflag = flags & bitmask;									\
		switch (currentflag)										\
		__VA_ARGS__											\
		flagschecked |= currentflag;									\
	}
#define getbitflag(flags, flag) (flags & flag) == flag	



 #define BLACK "\033[0;30m"
 #define RED "\033[0;31m"
 #define GREEN "\033[0;32m"
 #define ORANGE "\033[0;33m"
 #define BLUE "\033[0;34m"
 #define PURPLE "\033[0;35m"
 #define CYAN "\033[0;36m"
 #define LIGHTGRAY "\033[0;37m"   
 #define DARKGRAY "\033[1;30m"   
 #define LIGHTRED "\033[1;31m"   
 #define LIGHTGREEN "\033[1;32m"   
 #define YELLOW "\033[1;33m"   
 #define LIGHTBLUE "\033[1;34m" 
 #define LIGHTPURPLE "\033[1;35m"
 #define LIGHTCYAN "\033[1;36m"
 #define WHITE "\033[1;37m"
 #define NC "\033[1;0m"


#define loop(loopvar, num) for(int loopvar = 0; loopvar < num; loopvar++)

#define loopat(loopvar, init, num) for(int loopvar = init; loopvar < num; loopvar++)

#define iter(array, size, type, item) 	\
type* item; 				\
for(int __##item##_iterator = 0; 	\
    __##item##_iterator < size; 	\
    item = &((type*)array)[__##item##_iterator++])

#define __VA_ARG_LEN__(type, ...)  sizeof((void*[]){__VA_ARGS__}) / sizeof(void*)







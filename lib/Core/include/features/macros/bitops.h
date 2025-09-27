
#define getbitflagsinit size_t currentflag = 0, flagschecked = 0; 

#define getbitflags(flags, ...)											\
	currentflag = 0;											\
	flagschecked = 0;											\
	for (size_t bitmask = 1;(flags - flagschecked) > 0; bitmask*=2) 					\
	{													\
		currentflag = flags & bitmask;									\
		switch (currentflag)										\
		__VA_ARGS__											\
		flagschecked |= currentflag;									\
	}
#define getbitflag(flags, flag) (flags & flag) == flag	

#include "basetypes.h"


#define atom(type) _Atomic(type)

typedef atom(u8)  mutex;
typedef atom(u32) semaphore;

#define busy(mutex,...) if(mutex){errvt err = ERR(ERR_BUSY, "data is currently in use elsewhere"); __VA_ARGS__;} mutex = true; \
			for(atom(u8)* busy = &(mutex); *busy; *busy = false) 

#define busy_return *busy = false; return 

#define while_busy(call) for(call; errnm == ERR_BUSY; call)

#pragma once


#define __XC_ERROR__
#include "pkg.h"

Decl(Loggable)

typedef enum{ 
	#define __ERROR_CODES__
	#include "../config.h"
	#undef __ERROR_CODES__
}XC_ERROR_CODES;


Type(LogBook,
	intf(Loggable) interface;
     	inst(Object) object;
);
Enum (LogType, LOGGER_ERROR, LOGGER_INFO)

Enum(ErrorSignal,
	SIG_FGPE = 1 , 
	SIG_ILL = 2  , 
	SIG_INT = 4  ,
	SIG_ABRT = 8 , 
	SIG_SEGV = 16
);

#define SIG_ALL (SIG_FGPE | SIG_ILL | SIG_INT | SIG_ABRT | SIG_SEGV)

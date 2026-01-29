#pragma once
#include <XC.Core/pkg.h>
#define module std, Error, Code

// this sucks but whatever it works for now
moduleValue(NONE) 	= 0;
moduleValue(NIL) 	= 1;
moduleValue(FAIL) 	= 2;
moduleValue(INVALID) 	= 3;
moduleValue(BUSY) 	= 4;
moduleValue(NOTIMPLEM) 	= 5;
moduleValue(INIT) 	= 6;

moduleValues(DATA,
	MEMALLOC,	  7,
	SIZETOOLARGE,	  8,
	OUTOFRANGE,	  9,
	LIMIT,		  10, 
	EMPTY,		  11, 	
	DSN,		  12,				

)

moduleValues(MEM,
	OVERFLOW			as 13,
    	OUT_OF_MEMORY			as 14,
    	METADATA_CAP_EXCEEDED		as 15,
    	HEAP_LIMIT_REACHED		as 16,
    	INVALID_POINTER			as 17,
    	DOUBLE_FREE			as 18,
    	CORRUPTION_DETECTED		as 19,
    	ALIGNMENT_FAILURE		as 20,
    	RELOCATION_FAILED		as 21,
    	INVALID_SIZE			as 22,
    	UNALIGNED_BASE			as 23,
    	INVALID_SETTINGS		as 24,
    	THREAD_SAFETY_VIOLATION		as 25,
    	REALLOC_FAILED			as 26,
    	QUARANTINE_FULL			as 27,
	RUNTIME_TUNING_DISABLED		as 28,
	INCOMPATIBLE_SETTINGS		as 29,
	LARGE_ALLOC_FAILED		as 30,
	ALLOC_FAILED			as 31,
)

moduleValues(STRING,
	ENCODING			as 32,
)

export(
SUBMODULE(), 
VALUES(
	NONE, NIL, FAIL, INVALID, INIT, NOTIMPLEM, BUSY,
	DATA, MEM, STRING
),
	
)

#undef module
#define module std, Error


export(
SUBMODULE(Code), 
VALUES(),
	Throw, Try, Get, Set, Show, Hide, SetOutput, Clear
)



#undef module


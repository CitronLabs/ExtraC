#pragma once
#include <Core/pkg.h>

#define module std, Error, Code
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
	OVERFLOW,,
)

moduleValues(STRING,
	ENCODING,,
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
)



#undef module


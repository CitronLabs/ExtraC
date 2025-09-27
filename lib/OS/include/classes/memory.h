#pragma once
#include "../OS.h"

Class(Memory, 
INIT(u32 num_of_pages; u8 protections), 
FIELD(void* mem),
	errvt method(Memory, changeProt,, u8 protections);
      	u32 (*getPageSize)();
)

Class(DynamicLib, 
INIT(char* path), 
FIELD(),
	void* method(DynamicLib, searchSymbol,, char* symbol);
)

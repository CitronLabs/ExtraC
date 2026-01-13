#pragma once
#include "../config.c"

struct XC_DataStructs{
	value_config maxSize; bool_config autoGrow;
}; 

static const struct XC_DataStructs 
__List = { 
	.maxSize 			= configMax(value),
	.autoGrow			= true
},
__Queue = { 
	.maxSize 			= configMax(value),
	.autoGrow			= true,
},
__Stack = { 
	.maxSize 			= configMax(value),
	.autoGrow			= true,
},
__Map = { 
	.maxSize 			= configMax(value),
	.autoGrow			= true
};


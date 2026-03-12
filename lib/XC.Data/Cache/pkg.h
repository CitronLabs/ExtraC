#pragma once
#define __XC_DATA__
#include "../pkg.h"

#include "../String/pkg.h"
#include "../Map/pkg.h"

#ifndef __XC_CACHE__
#define package std

typefrom(word, Cache_ID);

Class(Cache,
INIT(
	len_t maxSize;
      	u16   cleanupCounter;
),
FMT(),
private(
	std_Map lookup;
)
){
	pkg(Cache_ID) 	method(Cache, Add, len_t size_in_bytes, std_String* label, void* data);
	void* 		method(Cache, Get, pkg(Cache_ID) id);
	void* 		method(Cache, Search, std_String* label);
      	errvt 		method(Cache, Clean);

};

#undef package
#endif

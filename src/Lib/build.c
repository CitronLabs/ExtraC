#include "Data/build.c"
#include "Storage/build.c"

#define module XC

source(include(
	Core,
	Data,
	Storage,
	Time,
	Alloc
))

#undef module


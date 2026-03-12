#pragma once
#define __XC_IO__
#include "../pkg.h"


#ifndef __XC_IO_CONSOLE__
#include "utils.h"

#include "CLI/pkg.h"

#define package std

Class(Console,
INIT(std_Stream *output, *input, *error; bool freeOnDestroy),
FMT(),
	std_Stream *output, *input, *error;
private(
	bool freeOnDestroy;
) 
){
	std_Console* fn(getDefault)();
};

#undef package
#endif

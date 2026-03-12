#pragma once
#define __XC_IO_CONSOLE__
#include "../pkg.h"


#ifndef __XC_IO_CONSOLE_CLI__
#define package std

Class(CLI,
INIT(),
FMT(),
     std_Stream stream;
){
	errvt method(CLI, getArg,  std_varData variable);
	errvt method(CLI, rewind,  len_t places);
	errvt method(CLI, advance, len_t places);
};

#undef package
#endif

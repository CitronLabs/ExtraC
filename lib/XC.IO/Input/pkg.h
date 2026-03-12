#pragma once
#define __XC_IO__
#include "../pkg.h"

#ifndef __XC_IO_INPUT__
#include "types.h"

#include "Keyboard/pkg.h"
#include "Mouse/pkg.h"

#define package std

Interface(Input,
	interface(std_Input_Keyboard) 	Keyboard;
	interface(std_Input_Mouse) 	Mouse;

)

#undef package
#endif

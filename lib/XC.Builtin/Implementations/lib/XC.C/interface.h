#pragma once
#define __XC_ENV_LIB_C__
#include "pkg.h"

#include "API/stddef.h"
#include "API/stdint.h"
#include "API/stdarg.h"
#include "API/stdlib.h"
#include "API/stdio.h"
#include "API/string.h"
#include "API/time.h"
#include "API/locale.h"
#include "API/errno.h"
#include "API/setjmp.h"
#include "API/signal.h"
#include "API/uchar.h"
#include "API/wctype.h"
#include "API/float.h"

Module(C){ 
	interface(C_IO)   	io;
	interface(C_GEN)  	lib;
	interface(C_TIME) 	time;
     	interface(C_LOCALE)   	locale;
  	interface(C_SETJMP) 	jmp;
  	interface(C_SIGNAL) 	sig;

	namespace(str,
  		interface(C_STR)   base;
  		interface(C_WSTR)  wide;
	)
};












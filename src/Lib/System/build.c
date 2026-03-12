#include <XC.Build/pkg.h>
#define module XC, System

source(,
	EnvVar.c,
       	Memory.c
       	Module.c,
       	Shell.c,
       	System.c
)

#undef module

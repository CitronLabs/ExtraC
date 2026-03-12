#include <XC.Build/pkg.h>

#define module XC, Storage, FS

source(,
	FS.c,
       	Dir/Dir.c,
       	File/File.c
       	Path/Path.c,
       	Entry.c,
)

#undef module


#define __ENV_COMMON_POSIX_IO__
#include "pkg.h"


#define package env_Common_Posix_IO

Class(File,
INIT(const char* path; 
     	struct {
	    u8 
     	    create : 1,
     	    read   : 1,
     	    write  : 1;
        } flags;
),
FMT(),
){
};

Class(Dir,
INIT(const char* path;
     	struct {
	    u8 
     	    create : 1,
     	    read   : 1,
     	    write  : 1;
        } flags;
),
FMT(),
){
};

#undef package

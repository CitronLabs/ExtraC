#pragma once
#define __XC_STORAGE_FS__
#include "pkg.h"

#define package std_FS

type_array(const char, Path, 256);

type(Info,
     	struct {
	const c8
     	    * seperator,
     	    * appdata,
     	    * usrroot,
     	    * sysroot;
     	} Path;
     	union {
	    u32 flags;
	    u32
     		directories 	: 1,
     		time_tracking 	: 1,
     		symlink     	: 1;
     	} Supports;
)

type(Entry_Info,
	const c8* name;
	const c8* path;
	len_t size;
	u64   time_created;
	u64   time_modified;
)


Data(Entry,
INIT(std_FS_Path name; Type_t type),
FMT(
	union{
	  struct{
		bool name : 1;
		bool size : 1;
		bool time_create : 1;
		bool time_modify : 1;
	  };
		bool detailed;
	};
	bool tree;
	bool pretty;
	len_t recursion_lvl;
	len_t tab_lvl;

),
    pkg(Entry_Info) info;
);

#undef package

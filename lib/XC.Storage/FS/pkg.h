#pragma once
#define __XC_STORAGE__
#include "../pkg.h"

#ifndef __XC_STORAGE_FS__
#include "types.h"

#include "File/pkg.h"
#include "Dir/pkg.h"

#define package std

Class(FS,
INIT(strc8 name; intf(device) interface),
FMT(),
	std_FS_Info info;
){
	interface(std_File) 	File;
	interface(std_Dir) 	Dir;

	std_FS* fn(getCurrent)();

	Process(Path, std_FS_Path,
	     	fn(join)(Array(FSPath) paths),
	     	fn(setExtension)(std_FS_Path ext),
	     	fn(rename)(std_FS_Path file),
	     	fn(move)(std_FS_Path file),
	     	fn(chdir)(std_FS_Path file),
	     	fn(resolve)(std_FS_Path file);

		std_String
			fn(getFileName)(),
		 	fn(getExtension)();

    		bool    fn(isAbsolute)(),
    			fn(isRelative)(),
    			fn(hasExtension)(std_FS_Path ext),
    			fn(isChildOf)(std_FS_Path parent_path);
	)
};

#undef package
#endif

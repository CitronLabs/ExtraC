#include "Path.h"
#define module std, FS, Path


Proc moduleFn(join)(Array(FSPath) paths){}
Proc moduleFn(setExtension)(std_FS_Path ext){}
Proc moduleFn(rename)(std_FS_Path file){}
Proc moduleFn(move)(std_FS_Path file){}
Proc moduleFn(chdir)(std_FS_Path file){}
Proc moduleFn(resolve)(std_FS_Path file){}

std_String moduleFn(getFileName)(){}
std_String moduleFn(getExtension)(){}

bool moduleFn(isAbsolute)(){}
bool moduleFn(isRelative)(){}
bool moduleFn(hasExtension)(std_FS_Path ext){}
bool moduleFn(isChildOf)(std_FS_Path parent_path){}

#undef module

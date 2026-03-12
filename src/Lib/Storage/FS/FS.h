#pragma once
#include <Env.pkg.c>
#include <XC.pkg.c>

from(std,
	Array_Stack as Stack
)

alias(sys.Device,   Dev)
alias(sys.Stream,   Stream)
alias(sys.Register, Reg)
alias(std.FS.Path,     Path)
alias(env.Devices.XC.Storage.Filesys, Filesys)



errvt std_FS_Dir_manualCopy(devHandle io_dev, streamHandle from, streamHandle to);
errvt std_FS_File_manualCopy(devHandle io_dev, streamHandle from, streamHandle to);

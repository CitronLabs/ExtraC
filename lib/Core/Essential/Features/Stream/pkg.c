#include <Core/pkg.h>


#define module std, Stream, Preset

export(
SUBMODULE(),
VALUES(), 
	staticBuffer,
	fromHandle
)


#undef module


#define module std, Stream, Process

export(
SUBMODULE(),
VALUES(), 
	start, cont, doEncode, doDecode,
	each, advance, rewind, readData,
	writeData, setProc, current,
	end, fail, doRun, pause, next
)


#undef module


#define module std, Stream

export(
SUBMODULE(Process, Preset),
VALUES(), 
	stdOut, stdIn, stdErr,
	
	getHandle,
	Flush,	
	ToPointer,
	GetPointer,
	GetCursorPos
)


#undef module

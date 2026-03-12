#pragma once
#define __XC_STORAGE_FS__
#include "../pkg.h"
#include "../types.h"


#ifndef __XC_STORAGE_FS_FILE__
#define package std

Class(File,
INIT(std_FS_Path path; uword flags),
FMT(),
	std_FS_Entry_Info info;
private(
	std_Stream stream;
	busylock lock;
)
){
	values(Flags, uword,
		WRITE,
		READ,
		APPEND,
		CREATE
	)

	values(CtrlCmd, uword,
		COPY,
		MOVE
	)
	std_Stream* method(File, getStream);

	errvt method(File, cpy,    std_FS_Path newPath);
	errvt method(File, move,   std_FS_Path newPath);
	errvt method(File, rename, std_FS_Path newName);
	errvt method(File, update);

	errvt method(File, delete, std_FS_Path atPath);
	errvt method(File, createAt, std_FS_Path atPath);
	errvt method(File, writeData, void* data, len_t len);
	errvt method(File, readData, void* data, len_t len);

	Process(Async, std_File,
		fn(cpy)(std_FS_Path newPath),
		fn(move)(std_FS_Path newPath),
		fn(rename)(std_FS_Path newName),
		fn(delete)(std_FS_Path atPath),
		fn(createAt)(std_FS_Path atPath),
		fn(writeData)(void* data, len_t len),
		fn(readData)(void* data, len_t len),
		fn(writeFmt)(std_Types_Format_Args args),
		fn(readFmt)(std_Types_Format_Args args),
	 	fn(waitFor)();

		bool fn(isDone)();
	 
	)
};

constructor(File, fromHandle, streamHandle handle);

#endif


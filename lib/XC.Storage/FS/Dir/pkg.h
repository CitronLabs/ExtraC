#pragma once
#define __XC_STORAGE_FS__
#include "../pkg.h"
#include "../types.h"


#ifndef __XC_STORAGE_FS_DIR__
#define package std

Class(Dir,
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
		CREATE
	)

	values(CtrlCmd, uword,
		COPY,
		MOVE
	)

	std_Stream* method(Dir, getStream);

	errvt method(Dir, cpy,    std_FS_Path newPath);
	errvt method(Dir, move,   std_FS_Path newPath);
	errvt method(Dir, rename, std_FS_Path newName);
	errvt method(Dir, update);

	errvt method(Dir, delete, std_FS_Path atPath);
	errvt method(Dir, createAt, std_FS_Path atPath);
	errvt method(Dir, addEntries, Array(std_FS_Entry*) entries);
	errvt method(Dir, readEntries, ArrayBuffer(std_FS_Entry*) buffer);

	Process(Async, std_Dir,
		fn(cpy)(std_FS_Path newPath),
		fn(move)(std_FS_Path newPath),
		fn(rename)(std_FS_Path newName),
		fn(delete)(std_FS_Path atPath),
		fn(createAt)(std_FS_Path atPath),
		fn(addEntries)(Array(std_FS_Entry*) entries),
		fn(readEntries)(ArrayBuffer(std_FS_Entry*) buffer),
	 	fn(waitFor)();

		bool fn(isDone)();
	 ;
	)

};

constructor(Dir, fromHandle, streamHandle handle);

#endif

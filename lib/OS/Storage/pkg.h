#pragma once
#define __XC_OS__
#include "../pkg.h"

#define package os

Class(File,
INIT(std_FSPath path; u16 flags; u16 char_size),
FMT(),
){
      	values(Flags, u16,
		ASYNC,
      		READ,
      		WRITE,
      		APPEND,
      		CREATE
      	)
	i64 		method(File, Read,     pntr output, len_t len);
	i64 		method(File, Write,    pntr input,  len_t len);
	errvt 		method(File, Move,     std_FSPath path);
	errvt 		method(File, Copy,     os_File* new_file, std_FSPath path);
	errvt 		method(File, Remove);
	errvt 		method(File, SetFlags, u16 flags);

};

Class(Dir,
INIT(strc8 path; u16 flags),
FMT(),
){
      	values(Flags, u16,
      		READ,
      		WRITE,
      		CREATE
      	)
	i64 	  method(Dir, Read,  std_FSEntry* output , u64 len);
	i64 	  method(Dir, Write, std_FSEntry* input , u64 len);
	errvt 	  method(Dir, Move,  std_FSPath path);
	errvt 	  method(Dir, Copy,  os_Dir* new_dir, std_FSPath path);
	errvt 	  fn(SetCurrent)(std_FSPath path);

};

#undef package

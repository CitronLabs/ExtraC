#pragma once
#define __XC_STREAM__
#include "../pkg.h"

#define package std

type_array(const char, FSPath, 256);

Data(FSEntry,
INIT(std_FSPath path; int flags),
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
	std_Stream* stream;
	char* name;
	char* path;
	len_t size;
	u64   time_created;
	u64   time_modified;
     	bool  isdir;
);

Interface(FS,
	values(Flags, int,
	   	CREATE, 
	   	APPEND, 
	   	WRITE, 
	   	READ, 
	   	RDWR, 
	   	DIR, 
	   	LINK 
	)
	std_Stream* 	fn(open)(std_FSPath path, int flags, std_FSEntry* ent_buff);
	std_Stream* 	fn(search)(std_FSPath path, std_FSEntry* ent);
	errvt  	 	fn(delete)(std_FSPath path);
	errvt  	 	fn(chdir)(std_FSPath path);
	errvt  	 	fn(update)(std_FSEntry* ent);
	errvt 		fn(rename)(std_FSPath path, std_FSPath new_name);
	errvt  	 	fn(move)(std_FSPath from, std_FSPath to);
	errvt  	 	fn(cpy)(std_FSPath from, std_FSPath to);
)

#undef package

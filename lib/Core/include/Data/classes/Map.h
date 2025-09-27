#pragma once

#define __XC_DATA__ 
#include "../pkg.h"
#include "../types.h"
#include "../utils.h"

Class(Map,
INIT(u64 init_size; typeData key, data; data_entry* literal),
FIELD(),

	u64 		method(Map, Count);
	errvt 		method(Map,Limit,, u64 limit);
	errvt 		method(Map,SetDefault,, void* data);
	List(data_entry)method(Map,GetEntries);
	errvt 		method(Map,Insert,, void* key, void* val);
	void* 		method(Map,Search,, void* key);
	u32 		method(Map,SearchIndex,, void* key);
	void* 		method(Map,Index,, u32 index);
	errvt 		method(Map,Remove,, void* key);
)


#define newMap(keyclass, datatype, ...) new(Map,	\
		sizeof((data_entry[]){__VA_ARGS__}) 	\
			/ sizeof(data_entry),		\
		typeclass(emptyobj(keyclass)),		\
		(data_entry[]){__VA_ARGS__})
#define pushMap(keyclass, datatype, ...) push(Map,	\
		sizeof((data_entry[]){__VA_ARGS__}) 	\
			/ sizeof(data_entry),		\
		(data_entry[]){__VA_ARGS__})


#define INVALID_MAPINDEX UINT32_MAX

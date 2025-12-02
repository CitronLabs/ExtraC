#pragma once

#define __XC_DATA__ 
#include "../pkg.h"
#include "../types.h"
#include "../utils.h"

#define package std

Class(Map,
INIT(u64 init_size; Type key, data; std_data_entry* literal),
FMT(bool debug),
private(
	std_typeData key, value;
	len_t allocednum;
	ifob(std_Allocator) allocator;
	List(data_entry) buckets;
	u32* bucket_indexes;
	u32 default_index;
)){
	u64 		method(Map, Count);
	errvt 		method(Map, Limit, u64 limit);
	errvt 		method(Map, SetDefault, void* data);
	List(data_entry)method(Map, GetEntries);
	errvt 		method(Map, Insert, void* key, void* val);
	void* 		method(Map, Search, void* key);
	u32 		method(Map, SearchIndex, void* key);
	void* 		method(Map, Index, u32 index);
	errvt 		method(Map, Remove, void* key);
	std_typeData	method(Map, GetValType);
	std_typeData	method(Map, GetKeyType);
};


#define newMap(keyType, valType, size) new(std_Map, size, T(keyType), T(valType))

#define pushMap(keyType, valType, size) push(std_Map, size, T(keyType), T(valType))

#define M(keyType, valType, ...) new(std_Map,		\
		sizeof((std_data_entry*[]){__VA_ARGS__})\
			/ sizeof(std_data_entry*),	\
		T(keyType), T(valType),			\
		(std_data_entry*[]){__VA_ARGS__})

#define m(keyType, valType, ...) push(std_Map,		\
		sizeof((std_data_entry*[]){__VA_ARGS__})\
			/ sizeof(std_data_entry*),	\
		T(keyType),T(valType),			\
		(std_data_entry*[]){__VA_ARGS__})

#define INVALID_MAPINDEX (maxof(u32))

#undef package

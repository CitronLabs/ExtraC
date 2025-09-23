#pragma once

#define __XC_DATA__ 
#include "../pkg.h"
#include "../types.h"
#include "../utils.h"

Class(Map,
__INIT(
      	u64 init_size; 
	u64 key_type_size;
	DSN_fieldType key_dsn_type;
	u64 data_type_size; 
	DSN_fieldType data_dsn_type;
	u32(*key_hash_func)(inst(Object) object);
	data_entry* literal;
),
__FIELD(),

	interface(Formatter);
	u64 		method(Map, Count);
	errvt 		method(Map,Limit,, u64 limit);
	errvt 		method(Map,SetDefault,, void* data);
	List(data_entry)method(Map,GetEntries);
	errvt 		method(Map,Insert,, void* key, void* val);
	void* 		method(Map,Search,, void* key);
	u32 		method(Map,SearchIndex,, void* key);
	void* 		method(Map,Index,, u32 index);
	errvt 		method(Map,Remove,, void* key);
	u32 (*Hash)(void* key, u64 len);
)


#define newMap(keyclass, datatype, ...) new(Map,	\
		sizeof((data_entry[]){__VA_ARGS__}) 	\
			/ sizeof(data_entry),		\
		sizeof(data(keyclass)),	 		\
		getDSN_Type((inst(keyclass)){0}),	\
		sizeof(datatype),			\
		getDSN_Type((datatype){0}),		\
		keyclass.__HASH,			\
		(data_entry[]){__VA_ARGS__})
#define pushMap(keyclass, datatype, ...) push(Map,	\
		sizeof((data_entry[]){__VA_ARGS__}) 	\
			/ sizeof(data_entry),		\
		sizeof(data(keyclass)),	 		\
		getDSN_Type((inst(keyclass)){0}),	\
		sizeof(datatype),			\
		getDSN_Type((datatype){0}),		\
		keyclass.__HASH,			\
		(data_entry[]){__VA_ARGS__})


#define INVALID_MAPINDEX UINT32_MAX

#pragma once

#define __XC_DATA__ 
#include "../pkg.h"
#include "../types.h"
#include "../utils.h"

Class(Struct,
INIT(data_entry* fields; u64 num_of_fields),
FIELD(Map(String, DSN_data) fields),

	errvt 		method(Struct, Define,, ...);
	errvt 		method(Struct, AddField,, cstr name, DSN_data* data);
	errvt 		method(Struct, Merge,, inst(Struct) datastruct);
	DSN_data*	method(Struct, SearchField,, inst(String) name);
)

#define pushStruct(...) 			\
	push(Struct, 				\
	     (data_entry[]){__VA_ARGS__}, 	\
	     sizeof((data_entry[]){__VA_ARGS__})\
	     / sizeof(data_entry)		\
	)

#define newStruct(...) 				\
	new(Struct, 				\
	     (data_entry[]){__VA_ARGS__}, 	\
	     sizeof((data_entry[]){__VA_ARGS__})\
	     / sizeof(data_entry)		\
	)


#define D(name, data) (data_entry){s(name), new(DSN_data, getDSN_Type(data), asObject(data))}

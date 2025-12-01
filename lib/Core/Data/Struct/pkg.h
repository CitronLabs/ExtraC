#pragma once

#define __XC_DATA__ 
#include "../pkg.h"
#include "../types.h"
#include "../utils.h"

#define package std

Class(Struct,
INIT(std_data_entry* fields; u64 num_of_fields),
FMT(),
	Map(String, DSN_data) fields;
){
	errvt 		method(Struct, Define, ...);
	errvt 		method(Struct, AddField, strc8 name, std_DSN_data* data);
	errvt 		method(Struct, Merge, std_Struct* datastruct);
	std_DSN_data*	method(Struct, SearchField, std_String* name);
};

#undef package

#define struct(...) 				\
	push(Struct, 				\
	     (data_entry[]){__VA_ARGS__}, 	\
	     sizeof((data_entry[]){__VA_ARGS__})\
	     / sizeof(data_entry)		\
	)

#define Struct(...) 				\
	new(Struct, 				\
	     (data_entry[]){__VA_ARGS__}, 	\
	     sizeof((data_entry[]){__VA_ARGS__})\
	     / sizeof(data_entry)		\
	)


#define D(name, data) (data_entry){s(name), new(DSN_data, getDSN_Type(data), asObject(data))}





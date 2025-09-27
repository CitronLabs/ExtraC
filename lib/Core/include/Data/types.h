#pragma once

#define __XC_DATA__ 
#include "pkg.h"

Decl(Number)
Decl(String)
Decl(List)
Decl(Stack)
Decl(Queue)
Decl(Map)
Decl(Struct)

/**
@enum DSN_fieldType
@brief Represents the different valid DSB types
*/
enum(DSN_fieldType,
	DSN_NULL,
	DSN_NUMBER,
	DSN_STRING,
	DSN_LIST,
	DSN_QUEUE,
	DSN_STACK,
	DSN_MAP,
	DSN_STRUCT
);

struct(DSN_data,
	DSN_fieldType type;
	union{
		void* 		data;
		inst(Number)	asNumber;
		inst(String)	asString;
		inst(List)  	asList;
		inst(Queue) 	asQueue;
		inst(Stack) 	asStack;
		inst(Map)   	asMap;
		inst(Struct)	asStruct;
     	};
);

struct(DSB_Header,
     	char magic[5];
	u16 version;
	u8 charlen;
     	u64 namelen; void* name;
	u64 body_size;
)
struct(DSB_Chain,
	DSN_fieldType ID;
     	u64 elmnt_size, len;
	void* data;
);
struct(DSB_Map,
	DSN_fieldType ID;
     	u64 keylen; void* key;
	u64 datalen; void* data;
);

#define entry(key, data) data_entry

struct(data_entry,
	void* key;
	void* data;
	u32 hash;
);

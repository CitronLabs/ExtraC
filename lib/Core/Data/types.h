#pragma once
#define __XC_DATA__ 
#include "pkg.h"

#define package std

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

type(DSN_data,
	std_DSN_fieldType type;
	union{
		void* 		data;
		std_Number*	asNumber;
		std_String*	asString;
		std_List*  	asList;
		std_Queue* 	asQueue;
		std_Stack* 	asStack;
		std_Map*   	asMap;
		std_Struct*	asStruct;
     	};
);

type(DSB_Header,
     	char magic[5];
	u16 version;
	u8 charlen;
     	u64 namelen; void* name;
	u64 body_size;
)
type(DSB_Chain,
	std_DSN_fieldType ID;
     	u64 elmnt_size, len;
	void* data;
);
type(DSB_Map,
	std_DSN_fieldType ID;
     	u64 keylen; void* key;
	u64 datalen; void* data;
);

#define entry(key, data) std_data_entry

type(data_entry,
	void* key;
	void* data;
	u32 hash;
);

#undef package

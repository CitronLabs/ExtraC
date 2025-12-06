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

#define entry(key, data) std_data_entry

type(data_entry,
	void* key;
	void* data;
	u32 hash;
);


#undef package
#define package std_DSN



/**
@enum DSN_fieldType
@brief Represents the different valid DSB types
*/
typefrom(word, FieldType);

type(Data,
	std_DSN_FieldType type;
	union{
		void* 		data;
		std_Number*	asNumber;
		std_String*	asString;
		std_List*  	asList;
		std_Map*   	asMap;
		std_Struct*	asStruct;
     	};
);

type(Binary_Header,
     	char magic[5];
	u16 version;
	u8 charlen;
     	u64 namelen; void* name;
	u64 body_size;
)
type(Binary_Chain,
	std_DSN_FieldType ID;
     	u64 elmnt_size, len;
	void* data;
);
type(Binary_Map,
	std_DSN_FieldType ID;
     	u64 keylen; void* key;
	u64 datalen; void* data;
);


#undef package

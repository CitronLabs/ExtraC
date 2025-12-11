#pragma once
#include "../../pkg.h"

import(std)

#define DSN_NONE   0
#define DSN_LIST   1
#define DSN_STRUCT 2
#define DSN_MAP    3
#define DSN_STRING 4
#define DSN_NUMBER 5

static inline std_DSN_FieldType resolveDSNType(std_typeData type){

	if(type.id == typeID(std_List))     { return std.DSN.Field.LIST;   }
	
	elif(type.id == typeID(std_Map))    { return std.DSN.Field.MAP;    }  
	
	elif(type.id == typeID(std_Struct)) { return std.DSN.Field.STRUCT; }

	elif(type.id == typeID(std_Number)) { return std.DSN.Field.NUMBER; }  
	
	elif(type.id == typeID(std_String)) { return std.DSN.Field.STRING; }

	else return std.DSN.Field.NONE;
}


static inline std_typeData TFromDSNType(std_DSN_FieldType type){

	if(type == std.DSN.Field.LIST)     { return T(std_List);   }
	                                         
	elif(type == std.DSN.Field.MAP)    { return T(std_Map);    }  
	             
	elif(type == std.DSN.Field.STRUCT) { return T(std_Struct); }

	elif(type == std.DSN.Field.NUMBER) { return T(std_Number); }  
	             
	elif(type == std.DSN.Field.STRING) { return T(std_String); }

	else return T(std_Nil);
}

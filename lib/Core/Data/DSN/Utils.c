#pragma once
#include "pkg.h"

static inline std_DSN_fieldType resolveDSNType(std_typeData type){

	if(type.id == typeID(std_List))        { return DSN_LIST;   }
	
	else if(type.id == typeID(std_Map))    { return DSN_MAP;    }  
	
	else if(type.id == typeID(std_Struct)) { return DSN_STRUCT; }

	else if(type.id == typeID(std_Number)) { return DSN_NUMBER; }  
	
	else if(type.id == typeID(std_String)) { return DSN_STRING; }

	else return DSN_NULL;
}


static inline std_typeData TFromDSNType(std_DSN_fieldType type){

	if(type == DSN_LIST)   	    { return T(std_List);   }
	                                         
	else if(type == DSN_MAP)    { return T(std_Map);    }  
	                                         
	else if(type == DSN_STRUCT) { return T(std_Struct); }

	else if(type == DSN_NUMBER) { return T(std_Number); }  
	                                         
	else if(type == DSN_STRING) { return T(std_String); }

	else return T(std_Nil);
}

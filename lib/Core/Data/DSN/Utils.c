#pragma once
#include "pkg.h"

static inline std_DSN_fieldType resolveDSNType(std_typeData type){

	if(generic type.id == typeID(std_Queue))	 { return DSN_QUEUE;  }
	
	else if(type.id == typeID(std_Stack))  { return DSN_STACK;  }
	
	else if(type.id == typeID(std_List))   { return DSN_LIST;   }
	
	else if(type.id == typeID(std_Map))    { return DSN_MAP;    }  
	
	else if(type.id == typeID(std_Struct)) { return DSN_STRUCT; }

	else return DSN_NULL;
}


static inline std_typeData typeDataFromDSNType(std_DSN_fieldType type){

	if(type == DSN_QUEUE)	    { return typeDataOf(std_Queue);  }
	                                         
	else if(type == DSN_STACK)  { return typeDataOf(std_Stack);  }
	                                        
	else if(type == DSN_LIST)   { return typeDataOf(std_List);   }
	                                         
	else if(type == DSN_MAP)    { return typeDataOf(std_Map);    }  
	                                         
	else if(type == DSN_STRUCT) { return typeDataOf(std_Struct); }

	else return typeDataOf(std_Nil);
}

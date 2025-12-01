#pragma once

#define __XC_DATA__ 
#include "../pkg.h"
#include "../types.h"
#include "../utils.h"

#include "../List/pkg.h"
#include "../Stack/pkg.h"
#include "../Queue/pkg.h"
#include "../Map/pkg.h"
#include "../Struct/pkg.h"

#define package std

#define DSB_Magic {'D','S','B',':','D'} //DSB:D
//
#define DSN_End    ((pntr)0)
#define DSN_Next   ((pntr)1)
#define DSN_Back   ((pntr)2)


Class(DSN, 
INIT(strc8 name; entry(String, DSN)* includes; len_t includes_num), 
FMT(),
	std_String* name; 
	std_Struct* body;
private(
	Map(String, u32) import_resolve;
	List(DSN)    imports;
	std_Struct*  header;
);
){
      	errvt 		method(DSN, include, std_DSN* include_data);
	std_DSN_data*	method(DSN, search, std_String* name);
	len_t 		method(DSN, parse,  std_DSN_data* ds, std_Stream* in);
	len_t 		method(DSN, format, std_DSN_data* ds, std_Stream* out);
	
	std_StreamDecoder decoder;
	std_StreamEncoder encoder;
	
	namespace(DSB,    
	    std_StreamDecoder decoder; 
	    std_StreamEncoder encoder;
	    len_t	method(DSN, format, std_DSN_data* ds, std_Stream* out);
	    len_t	method(DSN, parse, std_DSN_data* ds, std_Stream* in);
	)

	namespace(List,   
	    std_StreamDecoder decoder; 
	    std_StreamEncoder encoder;
	    len_t method(DSN, format, std_List* data, std_Stream* out);
	    len_t method(DSN, parse, std_List** data, std_Stream* out);
	)

	namespace(Stack,   
	    std_StreamDecoder decoder; 
	    std_StreamEncoder encoder;
	    len_t method(DSN, format, std_Stack* data, std_Stream* out);
	    len_t method(DSN, parse, std_Stack** data, std_Stream* out);
	)

	namespace(Queue,   
	    std_StreamDecoder decoder; 
	    std_StreamEncoder encoder;
	    len_t method(DSN, format, std_Queue* data, std_Stream* out);
	    len_t method(DSN, parse, std_Queue** data, std_Stream* out);
	)

	namespace(Map,   
	    std_StreamDecoder decoder; 
	    std_StreamEncoder encoder;
	    len_t method(DSN, format, std_Map* data, std_Stream* out);
	    len_t method(DSN, parse, std_Map** data, std_Stream* out);
	)

	namespace(Struct,   
	    std_StreamDecoder decoder; 
	    std_StreamEncoder encoder;
	    len_t method(DSN, format, std_Struct* data, std_Stream* out);
	    len_t method(DSN, parse, std_Struct** data, std_Stream* out);
	)

	namespace(Number,   
	    std_StreamDecoder decoder; 
	    std_StreamEncoder encoder;
	    len_t method(DSN, format, std_Number* data, std_Stream* out);
	    len_t method(DSN, parse, std_Number** data, std_Stream* out);
	)

	namespace(String,   
	    std_StreamDecoder decoder; 
	    std_StreamEncoder encoder;
	    len_t method(DSN, format, std_String* data, std_Stream* out);
	    len_t method(DSN, parse, std_String** data, std_Stream* out);
	)
};

#undef package

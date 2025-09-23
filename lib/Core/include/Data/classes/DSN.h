#pragma once

#define __XC_DATA__ 
#include "../pkg.h"
#include "../types.h"
#include "../utils.h"


#define DSB_Magic {'D','S','B',':','D'} //DSB:D

Class(DSN, 
__INIT(cstr name; inst(String) source; List(entry(String,DSN)) imports), 
__FIELD(inst(String) name; inst(Struct) body),
	
 	interface(Formatter);
	
      	errvt 		method(DSN, addImport,,   cstr name, inst(DSN) import_data);
	DSN_data*	method(DSN, searchField,, inst(String) name);
	u64		method(DSN, parseField,,  DSN_data* ds, inst(String) in);
	u64		method(DSN, formatField,, DSN_data* ds, inst(StringBuilder) out);
	
	// if buff is null then it return the expected size of the buff
	u64		method(DSN, formatDSB,, void* buff);
	u64		method(DSN, parseDSB,, void* buff, u64 size);
	
	u64 	method(DSN, parseList,,    inst(List)* data,	inst(String) in);
	u64 	method(DSN, parseQueue,,   inst(Queue)* data,	inst(String) in);
	u64 	method(DSN, parseStack,,   inst(Stack)* data,	inst(String) in);
	u64 	method(DSN, parseMap,,     inst(Map)* data,	inst(String) in);
	u64 	method(DSN, parseStruct,,  inst(Struct)* data,	inst(String) in);
	u64 	method(DSN, parseString,,  inst(String)* data,	inst(String) in);
	u64 	method(DSN, parseNumber,,  inst(Number)* data,	inst(String) in);
	
	u64 	method(DSN, formatList,,   inst(List) data,	inst(StringBuilder) out);
	u64 	method(DSN, formatQueue,,  inst(Queue) data,	inst(StringBuilder) out);
	u64 	method(DSN, formatStack,,  inst(Stack) data,	inst(StringBuilder) out);
	u64 	method(DSN, formatMap,,    inst(Map) data,	inst(StringBuilder) out);
	u64 	method(DSN, formatStruct,, inst(Struct) data,	inst(StringBuilder) out);
	u64 	method(DSN, formatString,, inst(String) data,	inst(StringBuilder) out);
	u64 	method(DSN, formatNumber,, inst(Number) data,	inst(StringBuilder) out);
)

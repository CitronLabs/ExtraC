#include "../../pkg.h"

import(std)


#include "Utils.c"

len_t parseListLikeDataStruct(std_DSN* self, std_Stream* in, std_varData data){
	
	len_t prev_size = size(in);

	std_DSN_fieldType first_type = DSN_NULL;
	std_DSN_data sub_field = {0};
	rune c = 0;

	std.Stream.Process
	    .start(in)
	    .doDecode(std.String.UTF8.Decoder, c){
		while(iswblank(c)) process->next();
		
		if(c == ']'){
			process->end();
			return prev_size - size(in);
		}


		if(!std.DSN.parse(self, &sub_field, in)){
			process->end();
			ERR(DATAERR_DSN, "failed to parse item");
			return 0;
		}

		if(sub_field.type == DSN_NULL){
			process->end();
			ERR(DATAERR_DSN, "invalid entry");
			return 0;
		}

		if(first_type == DSN_NULL){
			first_type = sub_field.type;

		}else if(first_type != sub_field.type) { 
			process->end();
			ERR(DATAERR_DSN, "multiple types not allowed in privs");
			return 0;
		}

		write_use(&data.type, data.data, sub_field.data);
		
		while(iswblank(c)) process->next();

		if(c != ','){
			process->end();
			ERR(DATAERR_DSN, "expected a ,");
			return 0;
		}
		}
	    then.end()
	;

ERR(DATAERR_DSN, "unexpected end of string");
return 0;
}

len_t methodimpl(std_DSN, parseQueue, std_Queue** data, std_Stream* in){

	len_t scanned_len = 0;

	if(!scanFrom(in, "<<[")){ 
		ERR(DATAERR_DSN, "invalid queue format");
		return 0;
	}
	
	*data = newQueue(pntr, 10);

	scanned_len += parseListLikeDataStruct(self, in, varDataOf(*data));
	
return scanned_len;
}

errvt QueueDSN_Decoder(std_Stream* strm, void* data){
	return std.DSN.Queue.parse(null, data, strm) == 0 ? 
		OK : ERR(ERR_FAIL, "failed to parse queue");
}

len_t methodimpl(std_DSN, parseStack, std_Stack** data, std_Stream* in){

	len_t scanned_len = 0;

	if(!scanFrom(in, ">>[")){ 
		ERR(DATAERR_DSN, "invalid stack format");
		return 0;
	}
	
	*data = newStack(pntr, 10);

	scanned_len += parseListLikeDataStruct(self, in, varDataOf(*data));
	
return scanned_len;
}

errvt StackDSN_Decoder(std_Stream* strm, void* data){
	return std.DSN.Stack.parse(null, data, strm) == 0 ? 
		OK : ERR(ERR_FAIL, "failed to parse stack");
}

len_t methodimpl(std_DSN, parseList, std_List** data, std_Stream* in){
	
	len_t scanned_len = 0;

	if(!scanFrom(in, "[")){ 
		ERR(DATAERR_DSN, "invalid list format");
		return 0;
	}
	
	*data = newList(pntr, 10);

	scanned_len += parseListLikeDataStruct(self, in, varDataOf(*data));
	
return scanned_len;
}

errvt ListDSN_Decoder(std_Stream* strm, void* data){
	return std.DSN.List.parse(null, data, strm) == 0 ? 
		OK : ERR(ERR_FAIL, "failed to parse list");
}

len_t methodimpl(std_DSN, parseMap, std_Map** data, std_Stream* in){

	rune c = 0;
	len_t prev_pos = size(in);
	std_DSN_fieldType first_types[2] = {0};
	std_DSN_data key = {0}, value = {0};
	std_data_entry currbucket = {0};
	ArrayList(data_entry) buckets = pushArrayList(std_data_entry, 10);

	std.Stream.Process
	    .start(in)
	    .doRun(1){
		if(!scanFrom(in, "@{")){
			ERR(DATAERR_DSN, "invalid map format");
			pop(buckets);
	       		return 0;
		}
	    }
	    then.doDecode(std.String.UTF8.Decoder, c){
		while(iswblank(c)) process->next();

	//SCANNING FOR KEY	
		
		if(!std.DSN.parse(self, &key, in)){
	       		ERR(DATAERR_DSN, "invalid DSN format");
			pop(buckets);
	       		return 0;
		}
		
		if(!scanFrom(in, "->")){
	       		ERR(DATAERR_DSN, "invalid DSN format");
			pop(buckets);
	       		return 0;

		}

		while(iswblank(c)) process->next();
		
	//SCANNING FOR DATA	
		
		if(!std.DSN.parse(self, &value, in)){
	       		ERR(DATAERR_DSN, "invalid DSN format");
			pop(buckets);
	       		return 0;
		}
		
	//VALIDATING FORMAT
		if(first_types[0] == DSN_NULL && first_types[1] == DSN_NULL)
			{first_types[0] = key.type; first_types[1] = value.type;}
	
		else if(first_types[0] != key.type || first_types[1] != value.type){
	       		ERR(DATAERR_DSN, "multiple types are not allowed in dsn maps");
			del(buckets);
	       		return 0;
		}


		currbucket.data = value.data;
		currbucket.key = key.data;

		write(buckets, &currbucket);

		while(iswblank(c)) process->next();
		
		if(c == ',')
			process->next();
		else if(c == '}') 
			break; 
	    }
	    then.end()
	;

	std_Map* map = new(std_Map, 
		     .key 	= typeDataFromDSNType(key.type),
		     .data	= typeDataFromDSNType(value.type),
		     .init_size 	= buckets->currSize,
		     .literal 		= buckets->data,
	);

	*data = map;

return prev_pos - size(in);
}

errvt MapDSN_Decoder(std_Stream* strm, void* data){
	return std.DSN.Map.parse(null, data, strm) == 0 ? 
		OK : ERR(ERR_FAIL, "failed to parse map");
}

len_t methodimpl(std_DSN, parseStruct, std_Struct** data, std_Stream* in){

	len_t prev_pos = size(in);
	std_Struct* result = new(std_Struct);

	std_DSN_data field = {0};
	rune c = 0;

	std.Stream.Process
	    .start(in)
	    .doRun(1){
		if(!scanFrom(in, "{")){
			ERR(DATAERR_DSN, "invalid map format");
			del(result);
	       		return 0;
		}
	    }
	    then.doDecode(std.String.UTF8.Decoder, c){

		while(iswblank(c)) process->next();

		if(!isalpha(c)){
			ERR(DATAERR_DSN, "invalid field name");
			del(result);
			return 0;
		}
		else if(c == '}'){
			break;
		}

		std_String* fieldName = new(std_String);

		if(!scanFrom(in, $(fieldName))){
			ERR(DATAERR_DSN, "invalid field name");
			del(result, fieldName); 
			return 0;
		}

		while(iswblank(c)) process->next();

		if(!scanFrom(in, "=")){
			ERR(DATAERR_DSN, "invalid struct format");
			del(result, fieldName);
			return 0;
		}

		while(iswblank(c)) process->next();

		if(!std.DSN.parse(self, &field, in)){
			ERR(DATAERR_DSN, "failed to parse DSN structure");
			del(result, fieldName); 
			return 0;
		}

		iferr(std.Map.Insert(result->fields, fieldName, &field)){
			ERR(DATAERR_DSN, "failed to add field");
			del(result, fieldName);
			return 0;
		}
	    }
	    then.end()
	;

	*data = result;

return prev_pos - size(in);
}

errvt StructDSN_Decoder(std_Stream* strm, void* data){
	return std.DSN.Struct.parse(null, data, strm) == 0 ? 
		OK : ERR(ERR_FAIL, "failed to parse struct");
}

len_t methodimpl(std_DSN, parseNumber, std_Number** data, std_Stream* in){
	
	len_t prev_pos = size(in);
	std_Number* result = new(std_Number);

	if(!scanFrom(in, $(result))){
		ERR(DATAERR_DSN, "failed to parse number");
		del(result);
		return 0;
	}

	*data = result;

return prev_pos - size(in);
}

errvt NumberDSN_Decoder(std_Stream* strm, void* data){
	return std.DSN.Number.parse(null, data, strm) == 0 ? 
		OK : ERR(ERR_FAIL, "failed to parse number");
}

len_t methodimpl(std_DSN, parseString, std_String** data, std_Stream* in){
	

	len_t prev_pos = size(in);
	std_String* result = new(std_String);

	if(!scanFrom(in, "\"", $(result), "\"")){
		ERR(DATAERR_DSN, "failed to parse string");
		del(result);
		return 0;
	};

	*data = result;

return prev_pos - size(in);
}

errvt StringDSN_Decoder(std_Stream* strm, void* data){
	return std.DSN.Struct.parse(null, data, strm) == 0 ? 
		OK : ERR(ERR_FAIL, "failed to parse string");
}

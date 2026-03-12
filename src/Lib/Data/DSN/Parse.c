#include <XC.Core/pkg.c>

#define module std, DSN

#include "Utils.c"

alias(std.Stream.Process, process)

len_t parseListLikeDataStruct(std_DSN* self, std_Stream* in, std_varData data){
	
	len_t prev_size = size(in);

	std_DSN_FieldType first_type = std_DSN_Field_NONE;
	std_DSN_Data sub_field = {0};
	rune c = 0;

	std.Stream.Process
	    .start(in)
	    .doDecode(std.String.Encoding.UTF8.Decoder, c){
		while(iswblank(c)) process.next();
		
		if(c == ']'){
			process.end();
			return prev_size - size(in);
		}


		if(!std.DSN.parse(self, &sub_field, in)){
			process.end();
			ERR(ERR.DATA.DSN, "failed to parse item");
			return 0;
		}

		if(sub_field.type == std_DSN_Field_NONE){
			process.end();
			ERR(ERR.DATA.DSN, "invalid entry");
			return 0;
		}

		if(first_type == std_DSN_Field_NONE){
			first_type = sub_field.type;

		}else if(first_type != sub_field.type) { 
			process.end();
			ERR(ERR.DATA.DSN, "multiple types not allowed in privs");
			return 0;
		}

		write_use(&data.type, data.data, sub_field.data);
		
		while(iswblank(c)) process.next();

		if(c != ','){
			process.end();
			ERR(ERR.DATA.DSN, "expected a ,");
			return 0;
		}
		}
	    process.end()
	;

ERR(ERR.DATA.DSN, "unexpected end of string");
return 0;
}

len_t moduleMethod(std_DSN, parseList, std_List** data, std_Stream* in){
	
	len_t scanned_len = 0;

	if(!scanFrom(in, "[")){ 
		ERR(ERR.DATA.DSN, "invalid list format");
		return 0;
	}
	
	*data = newList(pntr, 10);

	scanned_len += parseListLikeDataStruct(self, in, V(*data));
	
return scanned_len;
}

errvt ListDSN_Decoder(std_Stream* strm, void* data){
	return std.DSN.List.parse(nil, data, strm) == 0 ? 
		OK : ERR(ERR.FAIL, "failed to parse list");
}

len_t moduleMethod(std_DSN, parseMap, std_Map** data, std_Stream* in){

	rune c = 0;
	len_t prev_pos = size(in);
	std_DSN_FieldType first_types[2] = {0};
	std_DSN_Data key = {0}, value = {0};
	std_data_entry currbucket = {0};
	ArrayList(data_entry) buckets = pushArrayList(std_data_entry, 10);

	std.Stream.Process
	    .start(in)
	    .run(({
		if(!scanFrom(in, "@{")){
			ERR(ERR.DATA.DSN, "invalid map format");
			pop(buckets);
	       		return 0;
		}
		run_continue;
	    }))
	    .doDecode(std.String.Encoding.UTF8.Decoder, c){
		while(iswblank(c)) process.next();

	//SCANNING FOR KEY	
		
		if(!std.DSN.parse(self, &key, in)){
	       		ERR(ERR.DATA.DSN, "invalid DSN format");
			pop(buckets);
	       		return 0;
		}
		
		if(!scanFrom(in, "->")){
	       		ERR(ERR.DATA.DSN, "invalid DSN format");
			pop(buckets);
	       		return 0;

		}

		while(iswblank(c)) process.next();
		
	//SCANNING FOR DATA	
		
		if(!std.DSN.parse(self, &value, in)){
	       		ERR(ERR.DATA.DSN, "invalid DSN format");
			pop(buckets);
	       		return 0;
		}
		
	//VALIDATING FORMAT
		if(first_types[0] == std_DSN_Field_NONE && first_types[1] == std_DSN_Field_NONE)
			{first_types[0] = key.type; first_types[1] = value.type;}
	
		else if(first_types[0] != key.type || first_types[1] != value.type){
	       		ERR(ERR.DATA.DSN, "multiple types are not allowed in dsn maps");
			del(buckets);
	       		return 0;
		}


		currbucket.data = value.data;
		currbucket.key = key.data;

		write(buckets, &currbucket);

		while(iswblank(c)) process.next();
		
		if(c == ',')
			process.next();
		else if(c == '}') 
			break; 
	    }
	    process.end()
	;

	std_Map* map = new(std_Map, 
		     .key 	= TFromDSNType(key.type),
		     .data	= TFromDSNType(value.type),
		     .init_size 	= buckets->items,
		     .literal 		= privof(buckets).data,
	);

	*data = map;

return prev_pos - size(in);
}

errvt MapDSN_Decoder(std_Stream* strm, void* data){
	return std.DSN.Map.parse(nil, data, strm) == 0 ? 
		OK : ERR(ERR.FAIL, "failed to parse map");
}

len_t moduleMethod(std_DSN, parseStruct, std_Struct** data, std_Stream* in){

	len_t prev_pos = size(in);
	std_Struct* result = new(std_Struct);

	std_DSN_Data field = {0};
	rune c = 0;

	std.Stream.Process
	    .start(in)
	    .run(({
		if(!scanFrom(in, "{")){
			ERR(ERR.DATA.DSN, "invalid map format");
			del(result);
	       		return 0;
		}
		run_continue;
	    }))
	    .doDecode(std.String.Encoding.UTF8.Decoder, c){

		while(iswblank(c)) process.next();

		if(!isalpha(c)){
			ERR(ERR.DATA.DSN, "invalid field name");
			del(result);
			return 0;
		}
		else if(c == '}'){
			break;
		}

		std_String* fieldName = new(std_String);

		if(!scanFrom(in, $(fieldName))){
			ERR(ERR.DATA.DSN, "invalid field name");
			del(result, fieldName); 
			return 0;
		}

		while(iswblank(c)) process.next();

		if(!scanFrom(in, "=")){
			ERR(ERR.DATA.DSN, "invalid struct format");
			del(result, fieldName);
			return 0;
		}

		while(iswblank(c)) process.next();

		if(!std.DSN.parse(self, &field, in)){
			ERR(ERR.DATA.DSN, "failed to parse DSN structure");
			del(result, fieldName); 
			return 0;
		}

		iferr(std.Map.Insert(result->fields, fieldName, &field)){
			ERR(ERR.DATA.DSN, "failed to add field");
			del(result, fieldName);
			return 0;
		}
	    }
	    process.end()
	;

	*data = result;

return prev_pos - size(in);
}

errvt StructDSN_Decoder(std_Stream* strm, void* data){
	return std.DSN.Struct.parse(nil, data, strm) == 0 ? 
		OK : ERR(ERR.FAIL, "failed to parse struct");
}

len_t moduleMethod(std_DSN, parseNumber, std_Number** data, std_Stream* in){
	
	len_t prev_pos = size(in);
	std_Number* result = new(std_Number);

	if(!scanFrom(in, $(result))){
		ERR(ERR.DATA.DSN, "failed to parse number");
		del(result);
		return 0;
	}

	*data = result;

return prev_pos - size(in);
}

errvt NumberDSN_Decoder(std_Stream* strm, void* data){
	return std.DSN.Number.parse(nil, data, strm) == 0 ? 
		OK : ERR(ERR.FAIL, "failed to parse number");
}

len_t moduleMethod(std_DSN, parseString, std_String** data, std_Stream* in){
	

	len_t prev_pos = size(in);
	std_String* result = new(std_String);

	if(!scanFrom(in, "\"", $(result), "\"")){
		ERR(ERR.DATA.DSN, "failed to parse string");
		del(result);
		return 0;
	};

	*data = result;

return prev_pos - size(in);
}

errvt StringDSN_Decoder(std_Stream* strm, void* data){
	return std.DSN.Struct.parse(nil, data, strm) == 0 ? 
		OK : ERR(ERR.FAIL, "failed to parse string");
}

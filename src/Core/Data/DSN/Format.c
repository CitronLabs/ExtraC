#include <Core/pkg.c>
#define module std, DSN


#include "Utils.c"

len_t moduleMethod(std_DSN, formatList, std_List* data, std_Stream* out){
	
	std_DSN_FieldType dsn_type = resolveDSNType(std.List.GetType(data));

	if(!dsn_type) return 0;

	len_t formated_len = write(out, ">>[ ", fmt_end);
	
	foreach(data, void, elmnt){
	    formated_len += std.DSN.format(self, &(std_DSN_Data){dsn_type, elmnt}, out);

	    if(elmnt_iterator + 1 < elements(data))
		formated_len += write(out, ", ", fmt_end);
	}

	formated_len += write(out, " ]", fmt_end);
	
return formated_len;
}

errvt ListDSN_Encoder(std_Stream* strm, void* data){
	return std.DSN.List.format(nil, data, strm) == 0 ? 
		OK : ERR(ERR.FAIL, "failed to format list");
}

len_t moduleMethod(std_DSN, formatMap, std_Map* data, std_Stream* out){
	
	std_DSN_FieldType 
		key_dsn_type = resolveDSNType(std.Map.GetKeyType(data)),
		val_dsn_type = resolveDSNType(std.Map.GetValType(data))
	;

	if(!key_dsn_type || !val_dsn_type) return 0;
	
	std_List* entries = std.Map.GetEntries(data); 
		
	len_t formated_len = write(out, "@{ ", fmt_end);

	foreach(entries, std_data_entry, bucket){
		formated_len += std.DSN.format(self, &(std_DSN_Data){key_dsn_type, bucket->key}, out);
		formated_len += write(out, " -> ", fmt_end);
		formated_len += std.DSN.format(self, &(std_DSN_Data){val_dsn_type, bucket->data}, out);
		formated_len += write(out, ", ", fmt_end);
  	}
	formated_len += write(out, " }", fmt_end);

return formated_len;
}

errvt MapDSN_Encoder(std_Stream* strm, void* data){
	return std.DSN.Map.format(nil, data, strm) == 0 ? 
		OK : ERR(ERR.FAIL, "failed to format map");
}

len_t moduleMethod(std_DSN, formatStruct, std_Struct* data, std_Stream* out){
	
	len_t formated_len = write(out, "{ ");
	
	List(std_data_entry) entries = std.Map.GetEntries(data->fields);

	foreach(entries, std_data_entry, entry){
		formated_len += write(out, $use(std_String_Type, entry->key), " = ", fmt_end);
		formated_len += std.DSN.format(self, entry->data, out);
		formated_len += write(out, ", ", fmt_end);
	}

	formated_len += write(out, " }", fmt_end);

return formated_len;
}

errvt StructDSN_Encoder(std_Stream* strm, void* data){
	return std.DSN.Struct.format(nil, data, strm) == 0 ? 
		OK : ERR(ERR.FAIL, "failed to format struct");
}


len_t moduleMethod(std_DSN, formatNumber, std_Number* data, std_Stream* out){

	return write(out, $(data));	
}

errvt NumberDSN_Encoder(std_Stream* strm, void* data){
	return std.DSN.Number.format(nil, data, strm) == 0 ? 
		OK : ERR(ERR.FAIL, "failed to format struct");
}

len_t moduleMethod(std_DSN, formatString, std_String* data, std_Stream* out){

	return write(out, "\"", $(data), "\"");
}

errvt StringDSN_Encoder(std_Stream* strm, void* data){
	return std.DSN.String.format(nil, data, strm) == 0 ? 
		OK : ERR(ERR.FAIL, "failed to format struct");
}


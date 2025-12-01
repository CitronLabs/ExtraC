#include "../../pkg.h"

import(std)


#include "Utils.c"

len_t methodimpl(std_DSN, formatQueue, std_Queue* data, std_Stream* out){
	
	std_DSN_fieldType dsn_type = resolveDSNType(std.Queue.GetType(data));

	if(!dsn_type) return 0;

	len_t formated_len = write(out, "<<[ ", fmt_end);
	
	foreach(data, void, elmnt){
	    formated_len += std.DSN.format(self, &(std_DSN_data){dsn_type, elmnt}, out);

	    if(elmnt_iterator + 1 < elements(data))
		formated_len += write(out, ", ", fmt_end);
	}

	formated_len += write(out, " ]", fmt_end);
	
return formated_len;
}

errvt QueueDSN_Encoder(std_Stream* strm, void* data){
	return std.DSN.Queue.format(null, data, strm) == 0 ? 
		OK : ERR(ERR_FAIL, "failed to format queue");
}


len_t methodimpl(std_DSN, formatStack, std_Stack* data, std_Stream* out){

	std_DSN_fieldType dsn_type = resolveDSNType(std.Stack.GetType(data));

	if(!dsn_type) return 0;

	len_t formated_len = write(out, ">>[ ", fmt_end);
	
	foreach(data, void, elmnt){
	    formated_len += std.DSN.format(self, &(std_DSN_data){dsn_type, elmnt}, out);

	    if(elmnt_iterator + 1 < elements(data))
		formated_len += write(out, ", ");
	}

	formated_len += write(out, " ]", fmt_end);
	
return formated_len;
}

errvt StackDSN_Encoder(std_Stream* strm, void* data){
	return std.DSN.Stack.format(null, data, strm) == 0 ? 
		OK : ERR(ERR_FAIL, "failed to format stack");
}

len_t methodimpl(std_DSN, formatList, std_List* data, std_Stream* out){
	
	std_DSN_fieldType dsn_type = resolveDSNType(std.List.GetType(data));

	if(!dsn_type) return 0;

	len_t formated_len = write(out, ">>[ ", fmt_end);
	
	foreach(data, void, elmnt){
	    formated_len += std.DSN.format(self, &(std_DSN_data){dsn_type, elmnt}, out);

	    if(elmnt_iterator + 1 < elements(data))
		formated_len += write(out, ", ", fmt_end);
	}

	formated_len += write(out, " ]", fmt_end);
	
return formated_len;
}

errvt ListDSN_Encoder(std_Stream* strm, void* data){
	return std.DSN.List.format(null, data, strm) == 0 ? 
		OK : ERR(ERR_FAIL, "failed to format list");
}

len_t methodimpl(std_DSN, formatMap, std_Map* data, std_Stream* out){
	
	std_DSN_fieldType 
		key_dsn_type = resolveDSNType(std.Map.GetKeyType(data)),
		val_dsn_type = resolveDSNType(std.Map.GetValType(data))
	;

	if(!key_dsn_type || !val_dsn_type) return 0;
	
	std_List* entries = std.Map.GetEntries(data); 
		
	len_t formated_len = write(out, "@{ ", fmt_end);

	foreach(entries, std_data_entry, bucket){
		formated_len += std.DSN.format(self, &(std_DSN_data){key_dsn_type, bucket->key}, out);
		formated_len += write(out, " -> ", fmt_end);
		formated_len += std.DSN.format(self, &(std_DSN_data){val_dsn_type, bucket->data}, out);
		formated_len += write(out, ", ", fmt_end);
  	}
	formated_len += write(out, " }", fmt_end);

return formated_len;
}

errvt MapDSN_Encoder(std_Stream* strm, void* data){
	return std.DSN.Map.format(null, data, strm) == 0 ? 
		OK : ERR(ERR_FAIL, "failed to format map");
}

len_t methodimpl(std_DSN, formatStruct, std_Struct* data, std_Stream* out){
	
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
	return std.DSN.Struct.format(null, data, strm) == 0 ? 
		OK : ERR(ERR_FAIL, "failed to format struct");
}


len_t methodimpl(std_DSN, formatNumber, std_Number* data, std_Stream* out){

	return write(out, $(data));	
}

errvt NumberDSN_Encoder(std_Stream* strm, void* data){
	return std.DSN.Number.format(null, data, strm) == 0 ? 
		OK : ERR(ERR_FAIL, "failed to format struct");
}

len_t methodimpl(std_DSN, formatString, std_String* data, std_Stream* out){

	return write(out, "\"", $(data), "\"");
}

errvt StringDSN_Encoder(std_Stream* strm, void* data){
	return std.DSN.String.format(null, data, strm) == 0 ? 
		OK : ERR(ERR_FAIL, "failed to format struct");
}



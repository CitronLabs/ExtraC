#include "../../pkg.h"

import(std)



errvt methodimpl(std_Struct, Define, ...){
	nonull(self, return err);

	va_list args;
	va_start(args, self);
	
	bool cont = true;
	if(self->fields == null)
		self->fields = newMap(std_String, std_DSN_data, 10);
	
	for(;;){
		std_data_entry entry = va_arg(args, std_data_entry);
		if(entry.data == null) break;
			
		std.Map.Insert(self->fields, entry.key, entry.data);
	}

return OK;
}

errvt methodimpl(std_Struct, Merge, std_Struct* merge_struct){
	nonull(merge_struct, return err);
	nonull(self, return err);


	List(data_entry) entries = std.Map.GetEntries(merge_struct->fields);

	foreach(entries, std_data_entry, ent){
		std_DSN_data* res = std.Struct.SearchField(self, ent->key);
		if(res == null){
		    if(std.Struct.AddField(self, ent->key, ent->data) != ERR_NONE){
			return ERR(DATAERR_MEMALLOC, "failed to merge field to struct");
		    }
		}else{
			return ERR(DATAERR_OUTOFRANGE, "struct merge conflict");
		}
	}

return OK;
}

errvt methodimpl(std_Struct, AddField, strc8 name, std_DSN_data* field){
	nonull(self, return err;);
	nonull(field->data, return err;);


	if(ERR_NONE != std.Map.Insert(self->fields, asString(name, 1024), field)){
		return ERR(DATAERR_MEMALLOC, "could not add field to datastructs");
	}

return OK;
}

std_DSN_data* methodimpl(std_Struct, SearchField, std_String* name){
	nonull(self, return null;);
	nonull(name, return null;);
	
	std_DSN_data* result = null;

	result = std.Map.Search(self->fields, name);

return result;
}
COPY(std_Struct){
	std_Struct* dest = where;

	if(create(std_Struct, dest, 
		.num_of_fields  = elements(self->fields),
		.fields		= 
			std.List.GetPointer(
			    std.Map.GetEntries(
				self->fields
			    ),
			    0
			)
	) == nil)
		{ ERR(ERR_FAIL, "failed to copy struct"); return nil; }

return where;
}

SIZE(std_Struct){
	if(elements)
		return elements(self->fields);
	else
		return sizeof(std_Struct); 
}

WRITE(std_Struct){
	return ops(std_Map_Type).Write(self->fields, data, size);
}

READ(std_Struct){
	return ops(std_Map_Type).Read(self->fields, data, size);
}

ITER(std_Struct){
	nonull(self, return null);

return index(self->fields, index);
}


DESTROY(std_Struct){
	nonull(self, return err);

	List(data_entry) entries = std.Map.GetEntries(self->fields);

	foreach(entries, std_data_entry, entry){
	    std_DSN_data* field = entry->data;

	    ops(((std_Object*)field->data)->__type).Destroy(field->data);
	}
return OK;
}

SCAN(std_Struct){
	
	std_Struct* result = null;
	u64 len = std.DSN.Struct.parse(null, &result, in);

	if(len == 0){
		ERR(DATAERR_DSN, "failed to scan for struct");
		return 0;
	}
	*self = *result;

return len;
}

PRINT(std_Struct){
	
return std.DSN.Struct.format(null, self, out);
}



construct(std_Struct,
FMT(),
DEF(),
	  .Set  = nilmethod,
	  .Hash = nilmethod
){

	self->fields = newMap(std_String, std_DSN_data, 10);

	if(arg.fields != null && arg.num_of_fields != 0){
	    loop(i, arg.num_of_fields){
		std.Map.Insert(self->fields, 
		 	arg.fields[i].key, 
		 	arg.fields[i].data
		);
	    }
	}

return self;
}

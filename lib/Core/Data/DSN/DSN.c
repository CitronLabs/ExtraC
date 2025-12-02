#include "../../pkg.h"

import(std)



#include "Format.c"
#include "Parse.c"
#include "DSB.c"

errvt handleListOverride(std_DSN_data* reference, std_DSN_data* override){
	std_List* ref = reference->data; 
	std_List* ovr = override->data;
	
	if(std.List.GetType(ref).id != std.List.GetType(ovr).id)
		return ERR(DATAERR_DSN, "override types dont match that of the reference");
	
	len_t ref_size = std.List.Size(ref), ovr_size = std.List.Size(ovr);

	if(ref_size > ovr_size){
		void* ref_ovr = std.List.GetPointer(ref, ovr_size);
		std.List.Append(ovr, ref_ovr, ref_size - ovr_size);
	}
return OK;
}
errvt handleMapOverride(std_DSN_data* reference, std_DSN_data* override){
	std_Map* ref = reference->data; 
	std_Map* ovr = override->data;

	if(std.Map.GetValType(ref).id != std.Map.GetValType(ovr).id ||
	   std.Map.GetKeyType(ref).id != std.Map.GetKeyType(ovr).id)
		return ERR(DATAERR_DSN, "override types dont match that of the reference");

	List(data_entry) ref_ents = std.Map.GetEntries(ref);
	
	foreach(ref_ents, std_data_entry, ent){
		if(std.Map.SearchIndex(ovr, ent->key) == INVALID_MAPINDEX){
			std.Map.Insert(ovr, ent->key, ent->data);
		}
	}
return OK;
}
errvt handleStructOverride(std_DSN_data* reference, std_DSN_data* override){
	std_Struct* ref = reference->data; 
	std_Struct* ovr = override->data;

	List(data_entry) entries = std.Map.GetEntries(ref->fields);

	foreach(entries, std_data_entry, ent){
		std_DSN_data* res = std.Struct.SearchField(ovr, ent->key);
		if(res == null){
		    if(std.Struct.AddField(ovr, ent->key, ent->data) != ERR_NONE){
			return ERR(DATAERR_DSN, "failed to add missing field to override");
		    }
		}
		if(res->type != (*(std_DSN_data**)ent->data)->type)
			return ERR(DATAERR_DSN, "override does not match reference type");
	}

return OK;
}

len_t resolveReference(std_DSN* self, std_DSN_data* ds, std_Stream* in){

	if(self == null){
		ERR(DATAERR_DSN, "unable to resolve reference due to no DSN instance being provided");
		return 0;
	}

	len_t prev_pos = size(in);

	std_StringBuilder* name_builder = push(std_StringBuilder);

	std_DSN_data* reference = null;
	rune c = 0;

	std.Stream.Process
	    .start(in)
	    .doDecode(std.String.UTF8.Decoder, c){
		
		while(iswalnum(c)){ write(name_builder, &c); process->next(); }

		reference = std.DSN.search(self, std.StringBuilder.GetStr(name_builder));
		
		if(!reference){
			ERR(DATAERR_DSN, "reference not found");
			pop(name_builder);
			return 0;
		}

		while(iswblank(c)) process->next();
	
		// Simply use the reference data as the data for the field;
		if(c == ',') break;

		if(c != ':') {
			ERR(DATAERR_DSN, "invalid overide syntax ':' not found");
			pop(name_builder);
			return 0;
		}

		while(iswblank(c)) process->next();
		
		std_DSN_data override = {0};
		errvt err = ERR_NONE;

		if(!std.DSN.parse(self, &override, in)){
			ERR(DATAERR_DSN, "could not parse reference override");
			return 0;
		}
		if(override.type != reference->type){
			ERR(DATAERR_DSN, "override type is not equal to reference type");
			return 0;
		}

		switch (override.type) {
		case DSN_LIST:{
			err = handleListOverride(reference, &override);
		break;}
		case DSN_MAP:{
			err = handleMapOverride(reference, &override);
		break;}
		case DSN_STRUCT:{
			err = handleStructOverride(reference, &override);
		break;}
		default: break;
		}

		if(err){
			ERR(DATAERR_DSN, "failed to handle override");
			return 0;
		}

		reference = &override;
	    }
	    then.end()
	;

	copy_use(std_DSN_data_Type, reference, ds);

return prev_pos - size(in);
}



len_t methodimpl(std_DSN, parse, std_DSN_data* ds, std_Stream* in){

	rune c = 0;

	std.Stream.Process
	    .start(in)
	    .doDecode(std.String.UTF8.Decoder, c){
		
		while(iswblank(c)) process->next(); 

		switch (c) {
		case '[':{
			ds->type = DSN_LIST;
			return std.DSN.List.parse(self, &ds->asList, in);
		break;}
		case '@':{
			ds->type = DSN_MAP;
			return std.DSN.Map.parse(self, &ds->asMap, in);
		break;}
		case '0' ... '9':{
			ds->type = DSN_NUMBER;
			return std.DSN.Number.parse(self, &ds->asNumber, in);
		break;}
		case '"':{
			ds->type = DSN_STRING;
			return std.DSN.String.parse(self, &ds->asString, in);
		break;}
		case '{':{
			ds->type = DSN_STRUCT;
			return std.DSN.Struct.parse(self, &ds->asStruct, in);
		break;}
		case 'a' ... 'z':
		case 'A' ... 'Z':{
			return resolveReference(self, ds, in);
		break;}
		default:{
			ERR(DATAERR_DSN, "unexpected token");
		break;}
		}

		break;
	    }
	    then.end();
return 0;
}

errvt DSN_Decoder(std_Stream* strm, void* data){
	return std.DSN.parse(null, data, strm) == 0 ? 
		OK : ERR(ERR_FAIL, "failed to parse DSN");
}

len_t methodimpl(std_DSN, format, std_DSN_data* ds, std_Stream* out){

	switch (ds->type) {
	case DSN_LIST:{
		return std.DSN.List.format(self,   ds->data, out);
	break;}
	case DSN_MAP:{
		return std.DSN.Map.format(self,    ds->data, out);
	break;}
	case DSN_NUMBER:{
		return std.DSN.Number.format(self, ds->data, out);
	break;}
	case DSN_STRING:{
		return std.DSN.String.format(self, ds->data, out);
	break;}
	case DSN_STRUCT:{
		return std.DSN.Struct.format(self, ds->data, out);
	break;}
	default:{	
		ERR(DATAERR_DSN, "unexpected dsn data type");
	break;}
	}

return 0;
}

errvt DSN_Encoder(std_Stream* strm, void* data){
	return std.DSN.format(null, data, strm) == 0 ? 
		OK : ERR(ERR_FAIL, "failed to format DSN");
}

errvt methodimpl(std_DSN, import, std_DSN* import_data){
	
	if(std.Map.SearchIndex(priv.import_resolve, import_data->name) != INVALID_MAPINDEX){
		return ERR(DATAERR_DSN, "name already in use by another import");
	}

	u32 index = std.List.Size(priv.imports);
	std.List.Append(priv.imports, &import_data, 1);
	
	std.Map.Insert(priv.import_resolve, import_data->name, &index);

return OK;	
}

std_DSN_data* methodimpl(std_DSN, search, std_String* name){
	
	std_String* view = std.String.View(name, 0, name->len);
	std_Struct* curr_struct = null;
	std_DSN_data* result = null;

	foreach(name, rune, c){
	    if(*c == '.' || c_iterator + 1 == name->len){
		std.String.ViewShift(view, c_iterator, 0); 

		if(curr_struct == null){

		    u32* index = std.Map.Search(priv.import_resolve, &view);

		    if(index != null){
			std_DSN* import = std.List.GetPointer(priv.imports, *index);
			curr_struct = import->body;
		    }else{
			curr_struct = priv.header;
		    }
		}

		result = std.Struct.SearchField(curr_struct, view);
	 	if(result == null){
			ERR(DATAERR_DSN, "unable to resolve reference");
			return null;
	    	}
		if(*c == '.'){
			if(result->type != DSN_STRUCT){
				ERR(DATAERR_DSN, "only structs ({}) can be subreferenced");
				return null;
			}
			curr_struct = result->data;
		}else{
		    	return result;
		}
	    } 
	}

ERR(DATAERR_DSN, "unexpected end of reference");
return null;
};


HASH(std_DSN){ return hash(self->name); }
SIZE(std_DSN){ return sizeof(std_DSN); }

COPY(std_DSN){
	nonull(self || where, return nil);

	std_DSN* dest = where;

	*dest = (std_DSN){
	    .name      = new_alloc(std_String),
	    .body      = new_alloc(std_Struct),
	    .__type    = std_DSN_Type,
	    .__private = {
	    	.imports   = new_alloc(std_List),
	    	.header    = new_alloc(std_Struct),
	    	.import_resolve = new_alloc(std_Map)

	    }
	};


	try(){
	    copy(self->name, dest->name);
	    copy(self->body, dest->body);

	    copy(priv.imports, dest->__private.imports);
	    copy(priv.header,  dest->__private.header);

	    copy(priv.import_resolve, dest->__private.import_resolve);
	}catch{
	    free(dest->name);
	    free(dest->body);

	    free(dest->__private.imports);
	    free(dest->__private.header);

	    free(dest->__private.import_resolve);

	    return nil;
	}
return where;
}

DESTROY(std_DSN){
	nonull(self, return err);

	return del(
	    priv.header, 
	    priv.import_resolve, 
	    priv.imports, 
	    self->body, 
	    self->name
	);
}

SET(std_DSN){
	nonull(self || value, return err);

return set(self->name, value);
}

READ(std_DSN){
	
	len_t num_resolved = size;

	loop(i, size){
		std_data_entry* query = data[i];

		query->data = std.DSN.search(self, query->key);
		
		if(!query->data) num_resolved--;
	}

return num_resolved;
}

WRITE(std_DSN){
	
	len_t num_included = size;

	loop(i, size){
		std_DSN* include = data[i];
		
		if(!include){ num_included--; continue; }

		iferr(std.DSN.include(self, include)){ num_included--; continue; }
	}
}

PRINT(std_DSN){

	len_t prev_pos = size(out);

	foreach(std.Map.GetEntries(priv.import_resolve), std_data_entry, import_entry){
		
		std_String* name  = import_entry->key;
		u64*        index = import_entry->data;
		std_DSN*    data  = index(priv.imports, *index); 

		if(!data) continue;

		write(out, "import ", $(name), "; ");
	}

	
	foreach(std.Map.GetEntries(priv.header->fields), std_data_entry, header_entry){
		
		std_String*   name  = import_entry->key;
		std_DSN_data* data  = import_entry->data;

		write(out, $(name), " = ", $use(std_DSN_data_Type, data), ", ");
	}

	write(out, $(self->body));

return size(out) - prev_pos;
}

SCAN(std_DSN){

	len_t prev_pos = size(in);
	rune c = 0;

	std_String* name   = push(std_String);
	std_Struct
		* header   = new(std_Struct),
		* body 	   = new(std_Struct); 

	std.Stream.Process
	    .start(in)
	    .doDecode(std.String.UTF8.Decoder, c){
	
		while(scanFrom(in, $(name), " = ")){
		    std_DSN_data field_data = {0};

		    if(!std.DSN.parse(self, &field_data, in)){
			ERR(DATAERR_DSN, "failed to parse field data");
		    	del(header, body);
		    	pop(name);

			process->fail();
		    	return 0;
		    }

		    iferr(std.Struct.AddField(header, name->data, &field_data)){
			ERR(DATAERR_DSN, "failed to add field to struct");
		    	del(header, body);
		    	pop(name);

			process->fail();
		    	return 0;
		    }			
		    
		    process->next();
		    while(iswblank(c)) process->next();

		    if(c != ','){
			ERR(DATAERR_DSN, "missing a ',' at the end of a header declaration");
		    	del(header, body);
		    	pop(name);

			process->fail();
		    	return 0;
		    }			
		}

		if(c == '{'){
		    if(!scanFrom(in, $(body))){
			ERR(DATAERR_DSN, "failed to parse body");
		    	del(header, body);
		    	pop(name);

			process->fail();
		    	return 0;
		    }
		}
	    }
	    then.end()
	;

	if(self->body) del(self->body);
	self->body = body;

	if(priv.header) del(priv.header);
	priv.header = header;

	pop(name);

return prev_pos - size(in);
}

construct(std_DSN, 
FMT(), 
DEF(),
){
	this.name = new(std_String, args->name, 255);
	this.body = new(std_Struct);

	priv.header 	 = new(std_Struct);
	priv.imports 	 = newList(std_DSN, arg.includes_num ? args->includes_num : 10);
	priv.import_resolve  = newMap(std_String, u64, 5);
	

	loop(i, arg.includes_num){
		
		if(arg.includes[i].key)
			std.Map.Insert(
				priv.import_resolve, 
				arg.includes[i].key, 
				&(u64){ std.List.Size(priv.imports) }
			);

		write(priv.imports, arg.includes[i].data);
	}

return self;

}
 

#include <XC.Core/pkg.c>

#define module std, Map


errvt moduleMethod(std_Map, SetDefault, void* data){
	nonull(self, data) { return err; }
	
	std_data_entry def_bucket = {
		.hash = 0,
		.key = nil,
		.data = calloc(1, priv.value.size),
	};

	if(!def_bucket.data) return ERR(
		ERR.DATA.MEMALLOC, "could not initialize default data bucket");


	memcpy(def_bucket.data, data, priv.value.size);

	std.List.Append(priv.buckets, &def_bucket, 1);

return OK;
}

u32 moduleMethod(std_Map, Insert, void* key_data, void* itemptr){
	nonull(self, key_data, itemptr){ return INVALID_MAPINDEX; }

	u32 index = INVALID_MAPINDEX;

	u64 place = 10;
	u8 placenum = 10;
	u32 
	 key = INVALID_MAPINDEX,
	 hashvalue =
	    isImpl(ops(&priv.key).Hash) ?
		hash_bytes(key_data, priv.key.size) 
		:
		ops(&priv.key).Hash(key_data);
	
	for(;;){
		if(priv.bucket_indexes[ hashvalue % place] == 0){
			key = (hashvalue % place);
			index = std.List.Size(priv.buckets);
			std.List.FillSlot(priv.buckets, &(std_data_entry){
				.hash = hashvalue,
				.data = calloc(1, priv.value.size),
				.key  = calloc(1, priv.key.size),
			});
			// we add 1 to the index since we want to avoid using index 0 
			// because we check for index 0 as an invalid state in the search function
			priv.bucket_indexes[key] = index + 1;
			memcpy(
				((std_data_entry*)std.List.GetPointer(
					priv.buckets, 
					index)
				)->key,
				key_data,
				priv.key.size);
			memcpy(
				((std_data_entry*)std.List.GetPointer(
					priv.buckets, 
					index)
				)->data,
				itemptr,
				priv.value.size);
			break;
		}
		place+=placenum;
		if(place >= (10*10000) || place >= priv.allocednum){
			priv.bucket_indexes = realloc(priv.bucket_indexes, 
			    (priv.allocednum * 2) * sizeof(u32));
			priv.allocednum *= 2;
		}	
	}
	
return index;
}

u32 moduleMethod(std_Map, SearchIndex, void* key){
	nonull(self, key){ return INVALID_MAPINDEX; }
	
	u64 place = 10;
	u8 placenum = 10;
	u32 hashvalue = 
	    isImpl(ops(&priv.key).Hash) ?
		hash_bytes(key, priv.key.size) 
		:
		ops(&priv.key).Hash(key);
	
	for(;;){
		if(priv.bucket_indexes[hashvalue%place] != 0){
	
			std_data_entry* bucket = 
				std.List.GetPointer(
					priv.buckets,
					// see insert function for reason why we substract 1
					priv.bucket_indexes[hashvalue%place] - 1 
				);
				
			if(bucket->hash == hashvalue)
					// see insert function for reason why we substract 1
				return priv.bucket_indexes[hashvalue%place] - 1;
		}
		place+=placenum;
		if(place >= (10*10000) || place >= priv.allocednum){
			break;
		}
	}

return priv.default_index;
}


void* moduleMethod(std_Map, Search, void* key){
	nonull(self, key){ return nil; }

return std.Map.Index(self, std.Map.SearchIndex(self, key));
}

void* moduleMethod(std_Map,Index, u32 key){
	nonull(self){ return nil; }

	void* result = nil;

	if(key > std.List.Size(priv.buckets)){
		ERR(ERR.DATA.OUTOFRANGE, "invalid key");
		return nil;
	}
	
	std_data_entry* buckets = std.List.GetPointer(priv.buckets, 0);
	
	if(buckets[key].hash == 0){
		ERR(ERR.DATA.OUTOFRANGE, "invalid key");
		return nil;
	}
	result = buckets[key].data;

return result;
}
errvt moduleMethod(std_Map, Remove, void* key){
	nonull(self) { return err; }

	u32 mindex = std.Map.SearchIndex(self, key);

	if(INVALID_MAPINDEX == mindex) 
		return ERR(ERR.DATA.EMPTY, "key index not found");
	
	std_data_entry* bucket = 
		std.List.GetPointer(
			priv.buckets, 
			mindex
		);
	if(bucket == nil) 
		ERR(ERR.DATA.OUTOFRANGE, "invalid key");
	free(bucket->data); 
	free(bucket->key);
	*bucket = (std_data_entry){0}; 
	std.List.SetFree(priv.buckets, mindex);

return OK;
}
List(data_entry) moduleMethod(std_Map, GetEntries){
	nonull(self){ return nil; }
return priv.buckets;
}

u64 moduleMethod(std_Map, Count){
	nonull(self){ return 0; }
return std.List.Size(priv.buckets);
}

errvt moduleMethod(std_Map, Limit, u64 limit){
	nonull(self){ return err; }
return std.List.Limit(priv.buckets, limit);
}

std_typeData	moduleMethod(std_Map, GetValType){nonull(self){ return T(std_Nil); } return priv.value; }
std_typeData	moduleMethod(std_Map, GetKeyType){nonull(self){ return T(std_Nil); } return priv.key; }

DESTROY(std_Map){
	nonull(self){ return err; }
	
	foreach(priv.buckets, std_data_entry, entry){
		if(entry->data && entry->key){
			free(entry->data);
			free(entry->key);
		}
	}
	
	del(priv.buckets);
	free(priv.bucket_indexes); 
return OK;

}


WRITE(std_Map){
	nonull(self, data){ return 0; }

	len_t len_added = size;

	loop(i, size){
	    std_data_entry* entry = data[i];
	    if(entry->key){
	    	iferr(std.Map.Insert(self, entry->key, entry->data)){
		    len_added--;
		}
	    }
	    else if(priv.default_index == INVALID_MAPINDEX){
	    	iferr(std.Map.SetDefault(self, entry->data)){
		    len_added--;
		}
	    }
	}

return len_added;
}

READ(std_Map){
	nonull(self, data){ return 0; }

	len_t len_gotten = size;

	loop(i, size){
	    std_data_entry* entry = data[i];
	    if(entry->key){
		entry->data = std.Map.Search(self, entry->key);
		if(entry->data == nil)
		    len_gotten--;
	    }
	    else{
		len_gotten--;
	    }
	}

return len_gotten;

return size;
}

COPY(std_Map){
	nonull(self, where){ return nil; }
	
	std_Map* dest = where;

	*dest = (std_Map){
		.__type    = std_Map_Type,
		.__private = pntr_shiftcpy(dest, sizeof(std_Map)) 
	};
	
	
	dest->__private.value 		= priv.value;
	dest->__private.key		= priv.key;
	dest->__private.allocator	= priv.allocator;
	dest->__private.allocednum	= priv.allocednum;
	dest->__private.default_index	= priv.default_index;
	dest->__private.buckets		= new_alloc(std_List);
	dest->__private.bucket_indexes  = calloc(priv.allocednum, sizeof(u32));
	memcpy(dest->__private.bucket_indexes, priv.bucket_indexes, priv.allocednum);

	copy(priv.buckets, dest->__private.buckets);

return where;
}

ITER(std_Map){
	nonull(self){ return nil; }
return index(priv.buckets, index);
}

SIZE(std_Map){
	nonull(self){ return 0; }

	if(elements){
		return std.Map.Count(self);
	} else {
		return sizeof(std_Map);
	}
}

SCAN(std_Map){
	nonull(self, in){ return 0; }
	
	std_Map* result = nil;
	u64 len = std.DSN.Map.parse(nil, &result, in);

	if(len == 0){
		ERR(ERR.DATA.DSN, "failed to scan for map");
		return 0;
	}
	*self = *result;

return len;
}

PRINT(std_Map){
	nonull(self, out){ return 0; }

	u64 formated_len = 0;
	if(format == nil || !format->debug)
		formated_len += std.DSN.Map.format(nil, self, out);

	void* data = priv.buckets;

	write(out, 
       		"(Map){",
		    ".data = ", $(data), ", ",
		    ".item_size = ", $use(std_typeData_Type, &priv.value), ", ",
       		"}",
       fmt_end);


return formated_len;
}

construct(std_Map, 
FMT(),
DEF(),
){
	nonull(self, args){ return nil; }

	u64 init_size = args->init_size == 0 ? 10 : args->init_size;
	
	priv.bucket_indexes 	= calloc(init_size + (init_size / 2), sizeof(u32));
	priv.buckets		= newList(std_data_entry, 10);
	priv.key 		= arg.key;
	priv.value 		= arg.data;
	priv.allocednum 	= init_size + (init_size / 2);
	priv.default_index  	= INVALID_MAPINDEX;

	if(args->literal){
	    loop(i, args->init_size){
		    if(args->literal[i].key){
			std.Map.Insert(self, arg.literal[i].key, args->literal[i].data);
		    }
		    else if(priv.default_index == INVALID_MAPINDEX){
			std.Map.SetDefault(self, arg.literal[i].data);
	    	}
	    }
	}
return self;
}

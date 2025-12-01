#include "../../pkg.h"
#include "../DSN/pkg.h"
#include "../config.c"

import(std)




#define insertIntoListAt(index, _data, len) \
	memcpy(&(((u8*)priv.data)[index * priv.type.size]), _data, priv.type.size * len);


errvt methodimpl(std_List, Limit, len_t limit_size){
	
	nonull(self, return err);
	nonull(self, return err);
	priv.limit = limit_size;
	
	if(priv.items > limit_size || priv.items_alloced > priv.limit){
	    // reallocating to fit with the limits new bounds
		priv.data = realloc(priv.data, priv.type.size * priv.limit);
		
		if(null == priv.data ) return ERR( 
			DATAERR_MEMALLOC, "failed to reallocate list");
	
		priv.items = priv.items > priv.limit ? limit_size : priv.items;
		priv.items_alloced = limit_size;
	}

return OK;
}

errvt methodimpl(std_List, Grow, len_t plus_amount){

	if(priv.items == priv.limit) return ERR(
		DATAERR_LIMIT, "limit has been reached for this list");

	if(priv.items_alloced + plus_amount > priv.limit){
		priv.items_alloced = priv.limit - priv.items_alloced;
	}else{
		priv.items_alloced += plus_amount;
	}
	priv.data = realloc(priv.data, priv.items_alloced * priv.type.size);
	
	if(null ==  priv.data ) return ERR(
		DATAERR_MEMALLOC , "failed to grow this list");

return OK;
}

errvt methodimpl(std_List, Reserve, bool exact, len_t amount){
	nonull(self, return err);

	errvt result = OK;

	if(exact)
		result = std_List_Grow(self, amount);
	else
		result = std_List_Grow(self, priv.items_alloced + (priv.items_alloced / 2) + amount);

return result;
}

errvt methodimpl(std_List, Append, void* in, len_t len){
	nonull(self, return err;)
	nonull(in, return err;)


	if(priv.items + len > priv.items_alloced){
		if(__List.autoGrow)
			std_List_Grow(self, priv.items_alloced + (priv.items_alloced / 2) + len);
		else
			return ERR(DATAERR_OUTOFRANGE, "grow the list to fit new data");
	}
	insertIntoListAt(priv.items, in, len)
	
	priv.items++;
return OK;
}

errvt methodimpl(std_List, SetFree, u64 index){
    	nonull(self, return err;)


	if(priv.free_slots_buff == null){
		priv.free_slots_buff = calloc(10, sizeof(len_t));
		priv.free_slots_buff_alloced = 10;
		if(priv.free_slots_buff == null){
			return ERR(DATAERR_MEMALLOC, "failed to allocate free_slots_buff");
		}
	}
	
	priv.free_slots_buff[priv.free_slots_buff_size] = index;
	priv.free_slots_buff_size++;

return OK;	
}
u64 methodimpl(std_List, FillSlot, void* in){
	nonull(self, return err;)
	
	u64 index = maxof(len_t);

	if(priv.free_slots_buff == null || priv.free_slots_buff_size == 0){
		index = priv.items;
		std.List.Append(self, in, 1);

	}else{
		index = priv.free_slots_buff[--priv.free_slots_buff_size];
	}
return index;
}

errvt methodimpl(std_List, Insert, len_t len, u64 index, void* in){

	nonull(self, return err);

	
	index = index == maxof(len_t) ? priv.items : index;	
	
	if(index > priv.items) return ERR(
		DATAERR_SIZETOOLARGE , "index out of range");
	
	if(priv.items == priv.limit) return ERR(
		DATAERR_LIMIT, "limit has been reached for this priv");
	
	if(priv.items + len > priv.limit)
		len = priv.limit - priv.items;
	
	if(priv.items + len > priv.items_alloced){
		if(__List.autoGrow)
			std_List_Grow(self, len + (priv.items_alloced / 2));
		else
			return ERR(DATAERR_OUTOFRANGE, "grow the list to fit new data");
	}
	
	if(index == priv.items){
		insertIntoListAt(priv.items, in, len)
		priv.items+=len;
	}else{
		len_t size_restoflist = (priv.items - index);
		void* tempstore = calloc(size_restoflist, priv.type.size);
		void* indexloc = &(((u8*)priv.data)[index * priv.type.size]);
		memcpy(tempstore, indexloc, size_restoflist * priv.type.size);
	
		priv.items -= size_restoflist;
		insertIntoListAt(index, in, len)
		
		priv.items += len;
		
		insertIntoListAt(priv.items, tempstore, size_restoflist)
		priv.items += size_restoflist;
		
		free(tempstore);
	}

return OK;
}

#define mergpriv merged_list->__private
errvt methodimpl(std_List, Merge, std_List* merged_list, u64 index){

	nonull(self, return err;)
	nonull(merged_list, return err;)

	errvt result = ERR_NONE;

	if(mergpriv.type.size != priv.type.size) return ERR(
		DATAERR_SIZETOOLARGE , "different item sizes"
	);

	if(index == maxof(len_t)){
		result = std.List.Append(self, mergpriv.data, mergpriv.items);
	}else{
		result = std.List.Insert(self, mergpriv.items, index, mergpriv.data);
	}

return result;
}

std_List* methodimpl(std_List, SubList, u64 index, len_t len){

	nonull(self, return null;)
	std_List* out_list = null;


	if(index >= priv.items) {
		ERR(DATAERR_OUTOFRANGE , "index out of range");
		return null;
	}

	if(len == maxof(len_t))len = priv.items - index;

	void* indexloc = &(((u8*)priv.data)[index * priv.type.size]);

	out_list = new(std_List, 
		.init_size = len,
		.type = priv.type,
		.literal = indexloc
	);

return out_list;
}

errvt methodimpl(std_List, Index, bool write, u64 index, len_t len, void* data){
	
	nonull(self, return err;)
	nonull(data, return err;)
	
	if(index + len > priv.items_alloced) 
		return ERR(DATAERR_OUTOFRANGE, "index out of range");

	if(write){
		void* loc = &(((u8*)priv.data)[index * priv.type.size]);

		memcpy(loc, data, priv.type.size * len);
		priv.items++;
	}else{
		void* loc = &(((u8*)priv.data)[index * priv.type.size]);
		
		memcpy(data, loc, priv.type.size);
	}

return OK;
}
void* methodimpl(std_List, GetPointer, u64 index){
	
	nonull(self, return null)

	void* result = null;


	if(index > priv.items_alloced) {
		ERR(DATAERR_EMPTY, "index out of range");
		return null;
	}

	result = &(((u8*)priv.data)[index * priv.type.size]);

return result; 
}

errvt methodimpl(std_List, Cast, Type new_type){
	nonull(self, return err)

	priv.items = (priv.items * priv.type.size) / new_type.size; 
	priv.limit = (priv.limit * priv.type.size) / new_type.size; 
	priv.data = realloc(priv.data, (priv.items + 10) * new_type.size);
	priv.items_alloced = priv.items + 10;
	priv.free_slots_buff_size = 0;
	priv.type = new_type;

return OK;
}
len_t methodimpl(std_List, Size){
	nonull(self, return maxof(len_t);)
	
return priv.items;
}
void methodimpl(std_List, Flush){
	nonull(self, return)
	
	priv.items = 0; 
}
u32 methodimpl(std_List,Pop, u32 num){
	nonull(self, return 0)

	if(num > priv.items) 
		num = priv.items;
	
	priv.items -= num;
return num;
}


void* methodimpl(std_List, FreeToPointer){
	nonull(self, return null);

	void* res = priv.data;
	free(self);

return res;
}

std_typeData methodimpl(std_List, GetType){
	nonull(self, return T(std_Nil));

return priv.type;
}


HASH(std_List){ return hash_bytes(priv.data, priv.items * priv.type.size); }

SIZE(std_List){ if(elements) return priv.items; else return sizeof(std_List); }

SET(std_List){ 

	std.List.Flush(self);

	if(value != null)
	   for(int i = 0; ((void**)value)[i]; i++)
		std.List.Append(self, ((void**)value)[i], 1);
		
return OK;
}

WRITE(std_List){
	loop(i, size)
	    iferr(std.List.Append(self, data[i], 1))
		return i;
return size;
}

READ(std_List){
	loop(i, size)
	    iferr(std.List.Index(self, LISTINDEX_READ, i, 1, data[i]))
		return i;
return size;
}

COPY(std_List){
	nonull(self, return nil);

	if(!create(std_List, 
	    where, 
	    	.type = priv.type, 
	    	.init_size = priv.items, 
	    	.literal = priv.data
	    )
	){ ERR(ERR_FAIL, "failed to copy list"); return nil; } 

return where;
}

DESTROY(std_List){
	nonull(self, return err);
	
	if(priv.data != null) free(priv.data); 
return OK;
}

PRINT(std_List){
	nonull(self, return 0);
	len_t formated_len = 0;

	if(!format || !format->debug)
	formated_len += std.DSN.List.format(null, self, out);
			
	formated_len += write(out, "(List){ ",
		 	".items = ", $(priv.items), ", ",
			".limit = ", $(priv.limit), ", ",
		 	".type  = ", $use(std_typeData_Type, &priv.type), ", ", 
		     	".data  = { ",
	fmt_end);
	
	loop(i, priv.items)
		formated_len += write(out, 
		  	$use(&priv.type, 
			     pntr_shiftcpy(priv.data, i + priv.type.size)
			), ", ",
		fmt_end);

	formated_len += write(out, " }\n}", fmt_end);


return formated_len;
}

SCAN(std_List){
	nonull(self, return 0);
	
	std_List* result = null;
	len_t len = std.DSN.List.parse(null, &result, in);

	if(len == 0){
		ERR(DATAERR_DSN, "failed to scan for list");
		return 0;
	}

	*self = *result;

return len;
}

ITER(std_List){
	return std.List.GetPointer(self, index);
}




construct(std_List,
FMT(),
DEF(),
){
	
	priv.items_alloced = arg.init_size  == 0 ? 1 : args->init_size;
        priv.items = 0;
        priv.type = args->type;
        priv.limit = __List.maxSize;
	

	if(null == (priv.data = calloc(priv.items_alloced, priv.type.size))) { 
		ERR(DATAERR_MEMALLOC, "failed to allocate list");
		return null;
	}
	
	if(arg.init_size && args->literal){
		memcpy(priv.data, args->literal, priv.type.size * args->init_size);
		priv.items = args->init_size;
	}
return self;
}

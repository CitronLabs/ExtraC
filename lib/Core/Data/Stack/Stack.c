#include "../../pkg.h"
#include "../DSN/pkg.h"
#include "Core/Essential/Features/Type/Alloc/pkg.h"


import(std)


#define MAX_STACK_SIZE UINT32_MAX






errvt methodimpl(std_Stack,Push, void* item, u64 num){
	nonull(self, return err;);
	nonull(item, return err;);
	

	if((priv.top + 1) >= priv.allocednum){ 
		priv.allocednum = (priv.allocednum * 2) + num;
		priv.start = realloc(priv.start, priv.allocednum * priv.type.size);
	}
	
	memcpy(&((u8*)priv.start)[priv.top * priv.type.size], item, priv.type.size * num);
	priv.top += num;

return OK;
}
errvt methodimpl(std_Stack,Pop, void* out, u64 num){
	nonull(self, return err);
	nonull(out, return err);

	if(0 == priv.top) return ERR(DATAERR_EMPTY, "stack is empty");
	
	num = priv.top < num ? priv.top : num;

	loop(i, num){
		priv.top--;
		memcpy(&((u8*)out)[i * priv.type.size], &((u8*)priv.start)[priv.top * priv.type.size], priv.type.size);
	}

return OK;
}

u64 methodimpl(std_Stack, Count){
	nonull(self, return UINT64_MAX);

return priv.top;
}
bool methodimpl(std_Stack, Check){
	nonull(self, return false);

return priv.top > 0;
}
void* methodimpl(std_Stack, ToPointer){
	nonull(self, return null);

return priv.start;
}


errvt methodimpl(std_Stack, Grow, u64 add_amount){
	nonull(self, return err);
	if(priv.allocednum + add_amount > priv.limit){
		priv.allocednum = priv.limit - priv.allocednum;
	}else{
		priv.allocednum += add_amount;
	}

	priv.start = realloc(priv.start, priv.allocednum * priv.type.size);
	
	if(null == priv.start) return ERR(
		DATAERR_MEMALLOC , "failed to grow this stack");

return OK;
}


errvt methodimpl(std_Stack, Reserve, bool exact, u64 amount){
	nonull(self, return err);

	errvt result = OK;

	if(exact)
		result = std_Stack_Grow(self, amount);
	else
		result = std_Stack_Grow(self, priv.allocednum + (priv.allocednum / 2) + amount);

return result;
}

errvt methodimpl(std_Stack, Limit, u64 limit){
	nonull(self, return err);

	priv.limit = limit;
	if(priv.allocednum > priv.limit){
		priv.allocednum = priv.limit;
		priv.start = realloc(priv.start, priv.limit);
		priv.top = 
			priv.top > priv.limit ? 
			priv.limit : priv.top;
	}

return OK;
}

errvt methodimpl(std_Stack, Index, bool write, u64 index, void* data){
	nonull(self, return err);

	if(index > priv.top) return ERR(
		DATAERR_OUTOFRANGE, "index is out of range"
	);

	if(write){
		memcpy(priv.start + (priv.type.size * index), data, priv.type.size);
	}else{
		memcpy(data, priv.start + (priv.type.size * index), priv.type.size);
	}

return OK;
}

void* methodimpl(std_Stack, FreeToPointer){
	nonull(self, return null);

	void* result = priv.start;
	free(self);

return result;
}
noFail 		methodimpl(std_Stack, Flush){ priv.top = 0; }
std_typeData	methodimpl(std_Stack, GetType){ return priv.type; }

HASH(std_Stack){ return hash_bytes(priv.start, priv.top * priv.type.size); }

SIZE(std_Stack){ if(elements) return priv.top; else return sizeof(std_Stack); }

SET(std_Stack){ 

	std.Stack.Flush(self);

	if(value != null)
	   for(int i = 0; ((void**)value)[i]; i++)
		std.Stack.Push(self, ((void**)value)[i], 1);
		
return OK;
}

WRITE(std_Stack){
	loop(i, size)
	    iferr(std.Stack.Push(self, data[i], 1))
		return i;
return size;
}

READ(std_Stack){
	loop(i, size)
	    iferr(std.Stack.Pop(self, data[i], 1))
		return i;
return size;
}

COPY(std_Stack){
	nonull(self, return nil);

	if(!create(std_Stack, 
	    where, 
	    	.type = priv.type, 
	    	.init_size = priv.top, 
	    	.literal = priv.start
	    )
	){ ERR(ERR_FAIL, "failed to copy queue"); return nil; } 

return where;
}

DESTROY(std_Stack){
	nonull(self, return err;);

	free(priv.start);

return OK;
}

SCAN(std_Stack){
	nonull(self, return 0);
	
	std_Stack* result = null;
	u64 len = std.DSN.Stack.parse(null, &result, in);

	if(len == 0){
		ERR(DATAERR_DSN, "failed to scan for stack");
		return 0;
	}
	*self = *result;

return len;
}

PRINT(std_Stack){
	nonull(self, return 0);

	u64 formated_len = 0;

	if(!format || !format->debug)
		formated_len += std.DSN.Stack.format(null, self, out);
	
	formated_len += 
	    write(out, "(Stack){",
	    	    ".data = ",      $(priv.start), 	", ", 
	    	    ".items = ",     $(priv.top), 	", ", 		
	    	    ".item_size = ", $(priv.type.size),
	    	"}",
	    fmt_end);
	
return formated_len;
}

ITER(std_Stack){
	return pntr_shiftcpy(priv.start, priv.top * priv.type.size); 
}
construct(std_Stack, 
FMT(), 
DEF(),
){
	if(arg.init_size > MAX_STACK_SIZE){ 
	    	ERR(DATAERR_SIZETOOLARGE, "initsize is too large and exceeds the max stack size");
		return null;
	}
	
	
	if(0 == arg.type.size){
		ERR(DATAERR_SIZETOOLARGE, "data size cannot be 0");
		return null;
	}
	
	u64 start_size = arg.init_size  == 0 ? 10 : arg.init_size;

	
	priv.start = calloc(start_size, arg.type.size);
	priv.top = 0;
	priv.type = arg.type;
	priv.allocednum = start_size;

	if(null == priv.start){
	      	ERR(DATAERR_MEMALLOC,"unable to allocate memory for stack data");
		return null;
	}
	if(arg.init_size && arg.literal){
		memcpy(priv.start, arg.literal, arg.type.size * arg.init_size);
		priv.top = arg.init_size;
	}

return self;
}

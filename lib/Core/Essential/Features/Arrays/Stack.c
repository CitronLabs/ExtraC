#include "includes.h"


errvt methodimpl(std_Array_Stack, Grow, u64 add_amount){
	nonull(self, return err);
	
	priv.allocSize += add_amount;

	priv.data = realloc(priv.data, priv.allocSize * this.typeSize);
	
	if(!priv.data) 
		return ERR(ERR_FAIL , "failed to grow this stack");

return OK;
}

void*  methodimpl(std_Array_Stack, ToPointer){ return priv.data; }

errvt  methodimpl(std_Array_Stack, Reserve, bool exact, u64 amount){
	nonull(self, return err);

	errvt result = OK;

	if(exact)
		result = std_Array_Stack_Grow
			(self, amount);
	else
		result = std_Array_Stack_Grow
			(self, (priv.allocSize / 2) + amount);

return result;
}

noFail methodimpl(std_Array_Stack, Clear){ this.items = 0; }

WRITE(std_Array_Stack){
	nonull(self, return err;);
	
	if((this.items + 1) >= priv.allocSize){ 
		priv.allocSize = (priv.allocSize * 2) + size;
		priv.data = realloc(priv.data, priv.allocSize * this.typeSize);
	}
	loop(i, size)
		memcpy(&((u8*)priv.data)[this.items * this.typeSize], data[i], this.typeSize);
		this.items += size;

return OK;
}

READ(std_Array_Stack){
	nonull(self, return err);

	if(0 == this.items) return ERR(DATAERR_EMPTY, "stack is empty");
	
	size = this.items < size ? this.items : size;

	loop(i, size){
		this.items--;
		memcpy(&((u8*)priv.data)[i * this.typeSize], &((u8*)priv.data)[this.items * this.typeSize], this.typeSize);
	}

return OK;
}

SIZE(std_Array_Stack){
	if(!self)
		return sizeof(std_Array_Stack);
	elif (elements)
	  	return this.items;
	else
	  	return this.items * this.typeSize;
}
errvt methodimpl(std_Array_Queue, Grow, u64 add_amount);
errvt methodimpl(std_Array_List, Grow, u64 add_amount);
errvt methodimpl(std_Array_Stack, Grow, u64 add_amount);

COPY(std_Array_Stack){
	
	std_Object* dest_obj = where;

	switchT(dest_obj->__type){
	caseT(List){
	    List* dest = where;

	    if(dest->typeSize != this.typeSize){
		ERR(ERR_INVALID, "type sizes dont match between copying arrays");
		return null;
	    }

	    
	    if(dest->items + this.items > privof(dest).allocSize)
			std_Array_List_Grow(dest, (privof(dest).allocSize / 2) + this.items);

	    memcpy(
		pntr_shiftcpy(privof(dest).data, this.typeSize * dest->items), 
		priv.data,
		this.items * this.typeSize);

	break;}
	caseT(Buffer){
	    Buffer* dest = where;

	    if(dest->typeSize != this.typeSize){
		ERR(ERR_INVALID, "type sizes dont match between copying arrays");
		return null;
	    }

	break;}
	caseT(Stack){
	    Stack* dest = where;

	    if(dest->typeSize != this.typeSize){
		ERR(ERR_INVALID, "type sizes dont match between copying arrays");
		return null;
	    }

	    if(dest->items + this.items > privof(dest).allocSize)
			std_Array_Stack_Grow(dest, (privof(dest).allocSize / 2) + this.items);

	    
	    memcpy(
		pntr_shiftcpy(privof(dest).data, this.typeSize * dest->items), 
		priv.data,
		this.items * this.typeSize);

	break;}
	caseT(Queue){
	    Queue* dest = where;

	    if(dest->typeSize != this.typeSize){
		ERR(ERR_INVALID, "type sizes dont match between copying arrays");
		return null;
	    }

	    len_t alloc_size = 
		(pntr_asVal(privof(dest).end) - pntr_asVal(privof(dest).start)) / this.typeSize;
	    
	    if(dest->items + this.items > alloc_size)
			std_Array_Queue_Grow(dest, (alloc_size / 2) + this.items);
	    
	    loop(i, this.items)
		write(dest, pntr_shiftcpy(priv.data, this.typeSize * i));
	    
	break;}
	defaultT{
		ERR(ERR_INVALID, "invalid copy destination type detected");
		return null;
	}
	}

	create(Stack, where,  
		.initSize = this.items,
	       	.typeSize = this.typeSize,
	       	.data = priv.data
	);

return where;
}

DESTROY(std_Array_Stack){
	free(priv.data);
return OK;
}
SET(std_Array_Stack){
	free(priv.data);

	create(std_Array_Stack, self, 
		.typeSize = this.typeSize,
		.initSize = this.items,
		.data = value
	);
return OK;
}
HASH(std_Array_Stack){
	return hash_bytes(priv.data, this.typeSize * this.items);
}
ITER(std_Array_Stack){
	if(index > this.items) index = this.items;

	return pntr_shiftcpy(priv.data, this.items - index);
}


PRINT(std_Array_Stack){

	return write(out, 
	      "(ArrayStack){ ",
	       		".size = ", 	$(this.items), ", ",
			".alloced = ", 	$(priv.allocSize),", ",
	       		".typeSize = ", $(this.typeSize), ", ",
	       		".data = ", 	$(priv.data),
	      " }", fmt_end);

}

construct(std_Array_Stack,
FMT(),
DEF(),
	.Print   = std_Array_Stack_Op_Print,
	.Create  = std_Array_Stack_Op_Create,
	.Copy 	 = std_Array_Stack_Op_Copy,
	.Size 	 = std_Array_Stack_Op_Size,
	.Destroy = std_Array_Stack_Op_Destroy,
	.Set	 = std_Array_Stack_Op_Set,
	.Hash	 = std_Array_Stack_Op_Hash,
	.Write   = std_Array_Stack_Op_Write,
	.Read 	 = std_Array_Stack_Op_Read,
	.Iter 	 = std_Array_Stack_Op_Iter,
	.Scan    = nilmethod,
){
	len_t alloc_size = arg.data ? 
	 			arg.initSize + (args->initSize / 2) :
	  			arg.initSize;

	priv.data = calloc(alloc_size, arg.typeSize);

	priv.allocSize = alloc_size;
	this.typeSize = arg.typeSize;

	if(arg.data){
		memcpy(priv.data, arg.data, args->initSize * args->typeSize);
	  	
	  	this.items = arg.initSize;
	}

return self;


}


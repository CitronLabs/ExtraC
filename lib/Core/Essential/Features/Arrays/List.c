#include "includes.h"

errvt methodimpl(std_Array_List, Grow, u64 add_amount){
	len_t realloc_len = (priv.allocSize + add_amount) * this.typeSize;
	priv.data = realloc(priv.data, realloc_len);

	if(!priv.data){
	    return ERR(ERR_FAIL, "failed to reallocate array");
	}

	priv.allocSize += (priv.allocSize / 2) + add_amount;

return OK;
}



void*  methodimpl(std_Array_List, ToPointer){
	


}
errvt  methodimpl(std_Array_List, Reserve, bool exact, u64 amount){




}
u64    methodimpl(std_Array_List, Count){



}
bool   methodimpl(std_Array_List, Check){



}
noFail methodimpl(std_Array_List, Clear);




WRITE(std_Array_List){

	if(this.items + size > priv.allocSize)
		std_Array_List_Grow(self, (priv.allocSize / 2) + this.items);
	    
	loop(i, size){
	    memcpy(
		pntr_shiftcpy(priv.data, (this.items + i) * this.typeSize),
		data[i],
		this.typeSize
	    );
	}

	this.items += size;

return size;
}

READ(std_Array_List){

	if(size > this.items) size = this.items;

	loop(i, size){
	    memcpy(
		data[i],
		pntr_shiftcpy(priv.data, (this.items + i) * this.typeSize),
		this.typeSize
	    );
	}

return size;
}

errvt methodimpl(std_Array_Queue, Grow, u64 add_amount);
errvt methodimpl(std_Array_List, Grow, u64 add_amount);
errvt methodimpl(std_Array_Stack, Grow, u64 add_amount);

COPY(std_Array_List){

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

	create(List, where,  
		.initSize = this.items,
	       	.typeSize = this.typeSize,
	       	.data = priv.data
	);
}

HASH(std_Array_List){
	
	return hash_bytes(priv.data, this.typeSize * this.items);
}

ITER(std_Array_List){
	if(index > this.items) return null;

	return pntr_shiftcpy(priv.data, (this.items + index) * this.typeSize);
}

SIZE(std_Array_List){
	return elements ? this.items : this.items * this.typeSize;
}

SET(std_Array_List){
	
	if(value == null)
		memset(priv.data, 0, priv.allocSize * this.typeSize);

	else
	    loop(i, this.items){
	    	memcpy(
	            pntr_shiftcpy(priv.data, i * this.typeSize),
	            value,
	            this.typeSize
	        );
	    }
return OK;
}

DESTROY(std_Array_List){

	free(priv.data);

return OK;
}

PRINT(std_Array_List){

	return write(out, 
	      "(ArrayList){ ",
	       	".size = ", 	$(this.items), ", ",
		".alloced = ", 	$(priv.allocSize),", ",
	       	".typeSize = ", $(this.typeSize), ", ",
	       	".data = ", 	$(priv.data),
	      " }", fmt_end);

}

construct(std_Array_List,
FMT(),
DEF(),
	.Print   = std_Array_List_Op_Print,
	.Create  = std_Array_List_Op_Create,
	.Copy 	 = std_Array_List_Op_Copy,
	.Size 	 = std_Array_List_Op_Size,
	.Destroy = std_Array_List_Op_Destroy,
	.Set	 = std_Array_List_Op_Set,
	.Hash	 = std_Array_List_Op_Hash,
	.Write   = std_Array_List_Op_Write,
	.Read 	 = std_Array_List_Op_Read,
	.Iter 	 = std_Array_List_Op_Iter,
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



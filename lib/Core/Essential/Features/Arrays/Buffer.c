#include "includes.h"

void*  methodimpl(std_Array_Buffer, ToPointer){
	


}
void*  methodimpl(std_Array_Buffer, FreeToPointer){



}
errvt  methodimpl(std_Array_Buffer, Reserve, bool exact, u64 amount){




}
u64    methodimpl(std_Array_Buffer, Count){



}
bool   methodimpl(std_Array_Buffer, Check){



}
noFail methodimpl(std_Array_Buffer, Clear);

WRITE(std_Array_Buffer){
	nonull(self, return err;);
	
	if(size > this.items) size = this.items;

	loop(i, size)
		memcpy(pntr_shiftcpy(this.data, i * this.typeSize), data[i], this.typeSize);

return size;
}

READ(std_Array_Buffer){
	nonull(self, return err);

	if(0 == this.items) return ERR(DATAERR_EMPTY, "stack is empty");
	
	size = this.items < size ? this.items : size;

	loop(i, size)
	    memcpy(&((u8*)this.data)[i * this.typeSize],
	           &((u8*)this.data)[this.items * this.typeSize], 
	           this.typeSize
	    );
	

return OK;
}

SIZE(std_Array_Buffer){
	if(!self)
		return sizeof(std_Array_Buffer);
	elif (elements)
	  	return this.items;
	else
	  	return this.items * this.typeSize;
}
errvt methodimpl(std_Array_Queue, Grow, u64 add_amount);
errvt methodimpl(std_Array_List, Grow, u64 add_amount);
errvt methodimpl(std_Array_Stack, Grow, u64 add_amount);

COPY(std_Array_Buffer){

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
		this.data,
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
		this.data,
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
		write(dest, pntr_shiftcpy(this.data, this.typeSize * i));
	    
	break;}
	defaultT{
		ERR(ERR_INVALID, "invalid copy destination type detected");
		return null;
	}
	}

	create(List, where,  
		.initSize = this.items,
	       	.typeSize = this.typeSize,
	       	.data = this.data
	);
}

DESTROY(std_Array_Buffer){
	free(this.data);
return OK;
}
SET(std_Array_Buffer){
	free(this.data);

	create(std_Array_Buffer, self, 
		.typeSize = this.typeSize,
		.initSize = this.items,
		.data = value
	);
return OK;
}
HASH(std_Array_Buffer){
	return hash_bytes(this.data, this.typeSize * this.items);
}
ITER(std_Array_Buffer){
	if(index > this.items) index = this.items;

	return pntr_shiftcpy(this.data, this.items - index);
}


PRINT(std_Array_Buffer){

	return write(out, 
	      "(ArrayBuffer){ ",
	       		".size = ", 	$(this.items), ", ",
	       		".typeSize = ", $(this.typeSize), ", ",
	       		".data = ", 	$(this.data),
	      " }", fmt_end);

}

construct(std_Array_Buffer,
FMT(),
DEF(),
	.Print   = std_Array_Buffer_Op_Print,
	.Create  = std_Array_Buffer_Op_Create,
	.Copy 	 = std_Array_Buffer_Op_Copy,
	.Size 	 = std_Array_Buffer_Op_Size,
	.Destroy = std_Array_Buffer_Op_Destroy,
	.Set	 = std_Array_Buffer_Op_Set,
	.Hash	 = std_Array_Buffer_Op_Hash,
	.Write   = std_Array_Buffer_Op_Write,
	.Read 	 = std_Array_Buffer_Op_Read,
	.Iter 	 = std_Array_Buffer_Op_Iter,
	.Scan    = nilmethod,
){
	this.data = calloc(arg.initSize, arg.typeSize);

	this.typeSize = arg.typeSize;

	if(arg.data){
		memcpy(this.data, arg.data, args->initSize * args->typeSize);
	  	
	  	this.items = arg.initSize;
	}

return self;


}


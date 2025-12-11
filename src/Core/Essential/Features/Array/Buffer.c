#include <Core/pkg.c>
#define module std, Array, Buffer

from(std_Array, 
     use(List),
     use(Queue),
     use(Stack),
     use(Buffer)
)


errvt std_Array_Queue_Grow(Queue* self, u64 add_amount);
errvt std_Array_Stack_Grow(Stack* self, u64 add_amount);
errvt std_Array_List_Grow(List* self, u64 add_amount);


void*  moduleMethod(Buffer, ToPointer){ return this.data;}

errvt  moduleMethod(Buffer, Reserve, bool exact, u64 amount){
	
	if(amount > this.items) return OK;

	void* old_buff  = this.data;
	len_t prev_size = this.items;

	create(std_Array_Buffer, self, 
		.typeSize = this.typeSize,
		.initSize = exact ? amount : (this.items / 2) + amount,
	);

	memcpy(this.data, old_buff, prev_size * this.typeSize);

return OK;
}

noFail moduleMethod(std_Array_Buffer, Clear){ memset(this.data, 0, this.items * this.typeSize); }

WRITE(Buffer){
	nonull(self){ return err; }
	
	if(size > this.items) size = this.items;

	loop(i, size)
		memcpy(pntr_shiftcpy(this.data, i * this.typeSize), data[i], this.typeSize);

return size;
}

READ(Buffer){
	nonull(self){ return err; }

	if(0 == this.items) return ERR(ERR.DATA.EMPTY, "stack is empty");
	
	size = this.items < size ? this.items : size;

	loop(i, size)
	    memcpy(&((u8*)this.data)[i * this.typeSize],
	           &((u8*)this.data)[this.items * this.typeSize], 
	           this.typeSize
	    );
	

return OK;
}

SIZE(Buffer){
	return elements ? this.items : this.items * this.typeSize;
}


COPY(std_Array_Buffer){

	std_Object* dest_obj = where;

	switchT(dest_obj->__type){
	caseT(List){
	    List* dest = where;

	    if(dest->typeSize != this.typeSize){
		ERR(ERR.INVALID, "type sizes dont match between copying arrays");
		return nil;
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
		ERR(ERR.INVALID, "type sizes dont match between copying arrays");
		return nil;
	    }

	break;}
	caseT(Stack){
	    Stack* dest = where;

	    if(dest->typeSize != this.typeSize){
		ERR(ERR.INVALID, "type sizes dont match between copying arrays");
		return nil;
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
		ERR(ERR.INVALID, "type sizes dont match between copying arrays");
		return nil;
	    }

	    len_t alloc_size = 
		(pntr_asVal(privof(dest).end) - pntr_asVal(privof(dest).start)) / this.typeSize;
	    
	    if(dest->items + this.items > alloc_size)
			std_Array_Queue_Grow(dest, (alloc_size / 2) + this.items);
	    
	    loop(i, this.items)
		write(dest, pntr_shiftcpy(this.data, this.typeSize * i));
	    
	break;}
	defaultT{
		ERR(ERR.INVALID, "invalid copy destination type detected");
		return nil;
	}
	}

return create(Buffer, where,  
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
	.Scan    = nil,
){
	this.data = calloc(arg.initSize, arg.typeSize);

	this.typeSize = arg.typeSize;

	if(arg.data){
		memcpy(this.data, arg.data, args->initSize * args->typeSize);
	  	
	  	this.items = arg.initSize;
	}

return self;


}


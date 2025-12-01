#include "../../../pkg.h"

import(std)
use(std_Array, List, Queue, Stack, Buffer)


WRITE(std_Array_List){

	if(self->currSize + size > self->allocSize){
	    len_t realloc_len = (self->allocSize + (self->allocSize / 2) + size) * self->typeSize;
	    self->data = realloc(self->data, realloc_len);
	    
	    if(!self->data){
		ERR(ERR_FAIL, "failed to reallocate array");
		return 0;
	    }

	    self->allocSize += (self->allocSize / 2) + size;
	}


	loop(i, size){
	    memcpy(
		pntr_shiftcpy(self->data, (self->currSize + i) * self->typeSize),
		data[i],
		self->typeSize
	    );
	}

	self->currSize += size;

return size;
}

READ(std_Array_List){

	if(size > self->currSize) size = self->currSize;

	loop(i, size){
	    memcpy(
		data[i],
		pntr_shiftcpy(self->data, (self->currSize + i) * self->typeSize),
		self->typeSize
	    );
	}

return size;
}

COPY(std_Array_List){

	std_Object* dest_obj = where;

	switchT(dest_obj->__type){
	caseT(std_Array_Buffer){
	    Buffer* dest = where;

	    if(dest->typeSize != this.typeSize){
		ERR(ERR_INVALID, "type sizes dont match between copying arrays");
		return null;
	    }

	break;}
	caseT(std_Array_Stack){
	    Stack* dest = where;

	    if(dest->typeSize != this.typeSize){
		ERR(ERR_INVALID, "type sizes dont match between copying arrays");
		return null;
	    }

	break;}
	caseT(std_Array_Queue){
	    Queue* dest = where;

	    if(dest->typeSize != this.typeSize){
		ERR(ERR_INVALID, "type sizes dont match between copying arrays");
		return null;
	    }

	    loop(i, this.currSize)
		write(dest, pntr_shiftcpy(this.data, this.typeSize * i));
	    
	break;}
	defaultT{
		ERR(ERR_INVALID, "invalid copy destination type detected");
		return null;
	}
	}

	create(List, where,  
		.initSize = self->currSize,
	       	.typeSize = self->typeSize,
	       	.data = self->data
	);
}

HASH(std_Array_List){
	
	return hash_bytes(self->data, self->typeSize * self->currSize);
}

ITER(std_Array_List){
	if(index > self->currSize) return null;

	return pntr_shiftcpy(self->data, (self->currSize + index) * self->typeSize);
}

SIZE(std_Array_List){
	return elements ? self->currSize : self->currSize * self->typeSize;
}

SET(std_Array_List){
	
	if(value == null)
		memset(self->data, 0, self->allocSize * self->typeSize);

	else
	    loop(i, self->currSize){
	    	memcpy(
	            pntr_shiftcpy(self->data, i * self->typeSize),
	            value,
	            self->typeSize
	        );
	    }
return OK;
}

DESTROY(std_Array_List){

	free(self->data);

return OK;
}

PRINT(std_Array_List){

	return write(out, 
	      "(ArrayList){ ",
	       	".size = ", 	$(self->currSize), ", ",
		".alloced = ", 	$(self->allocSize),", ",
	       	".typeSize = ", $(self->typeSize), ", ",
	       	".data = ", 	$(self->data),
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

	self->data = calloc(alloc_size, arg.typeSize);

	self->allocSize = alloc_size;
	self->typeSize = arg.typeSize;

	if(arg.data){
		memcpy(self->data, arg.data, args->initSize * args->typeSize);
	  	
	  	self->currSize = arg.initSize;
	}

return self;
}



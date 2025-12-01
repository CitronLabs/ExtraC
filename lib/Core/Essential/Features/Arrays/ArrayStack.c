#include "../../../../pkg.h"

import(std)


errvt methodimpl(std_ArrayStack, Grow, u64 add_amount){
	nonull(self, return err);
	
	self->allocSize += add_amount;

	self->data = realloc(self->data, self->allocSize * self->typeSize);
	
	if(!self->data) 
		return ERR(ERR_FAIL , "failed to grow this stack");

return OK;
}



WRITE(std_ArrayStack){
	nonull(self, return err;);
	
	if((self->currSize + 1) >= self->allocSize){ 
		self->allocSize = (self->allocSize * 2) + size;
		self->data = realloc(self->data, self->allocSize * self->typeSize);
	}
	loop(i, size)
		memcpy(&((u8*)self->data)[self->currSize * self->typeSize], data[i], self->typeSize);
		self->currSize += size;

return OK;
}

READ(std_ArrayStack){
	nonull(self, return err);

	if(0 == self->currSize) return ERR(DATAERR_EMPTY, "stack is empty");
	
	size = self->currSize < size ? self->currSize : size;

	loop(i, size){
		self->currSize--;
		memcpy(&((u8*)self->data)[i * self->typeSize], &((u8*)self->data)[self->currSize * self->typeSize], self->typeSize);
	}

return OK;
}

SIZE(std_ArrayStack){
	if(!self)
		return sizeof(std_ArrayStack);
	elif (elements)
	  	return self->currSize;
	else
	  	return self->currSize * self->typeSize;
}
COPY(std_ArrayStack){

	std_ArrayStack* dest = where;

	create(std_ArrayStack, where, 
		.typeSize = self->typeSize,
		.initSize = self->currSize,
		.data = self->data
	);

return where;
}

DESTROY(std_ArrayStack){
	free(self->data);
return OK;
}
SET(std_ArrayStack){
	free(self->data);

	create(std_ArrayStack, self, 
		.typeSize = self->typeSize,
		.initSize = self->currSize,
		.data = value
	);
return OK;
}
HASH(std_ArrayStack){
	return hash_bytes(self->data, self->typeSize * self->currSize);
}
ITER(std_ArrayStack){
	if(index > self->currSize) index = self->currSize;

	return pntr_shiftcpy(self->data, self->currSize - index);
}


PRINT(std_ArrayStack){

	return write(out, 
	      "(ArrayStack){ ",
	       		".size = ", 	$(self->currSize), ", ",
			".alloced = ", 	$(self->allocSize),", ",
	       		".typeSize = ", $(self->typeSize), ", ",
	       		".data = ", 	$(self->data),
	      " }", fmt_end);

}

construct(std_ArrayStack,
FMT(),
DEF(),
	.Print   = std_ArrayStack_Op_Print,
	.Create  = std_ArrayStack_Op_Create,
	.Copy 	 = std_ArrayStack_Op_Copy,
	.Size 	 = std_ArrayStack_Op_Size,
	.Destroy = std_ArrayStack_Op_Destroy,
	.Set	 = std_ArrayStack_Op_Set,
	.Hash	 = std_ArrayStack_Op_Hash,
	.Write   = std_ArrayStack_Op_Write,
	.Read 	 = std_ArrayStack_Op_Read,
	.Iter 	 = std_ArrayStack_Op_Iter,
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


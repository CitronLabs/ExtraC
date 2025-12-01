#include "../../../pkg.h"

import(std)


errvt methodimpl(std_Array_Stack, Grow, u64 add_amount){
	nonull(self, return err);
	
	self->allocSize += add_amount;

	self->data = realloc(self->data, self->allocSize * self->typeSize);
	
	if(!self->data) 
		return ERR(ERR_FAIL , "failed to grow this stack");

return OK;
}



WRITE(std_Array_Stack){
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

READ(std_Array_Stack){
	nonull(self, return err);

	if(0 == self->currSize) return ERR(DATAERR_EMPTY, "stack is empty");
	
	size = self->currSize < size ? self->currSize : size;

	loop(i, size){
		self->currSize--;
		memcpy(&((u8*)self->data)[i * self->typeSize], &((u8*)self->data)[self->currSize * self->typeSize], self->typeSize);
	}

return OK;
}

SIZE(std_Array_Stack){
	if(!self)
		return sizeof(std_Array_Stack);
	elif (elements)
	  	return self->currSize;
	else
	  	return self->currSize * self->typeSize;
}
COPY(std_Array_Stack){

	std_Array_Stack* dest = where;

	create(std_Array_Stack, where, 
		.typeSize = self->typeSize,
		.initSize = self->currSize,
		.data = self->data
	);

return where;
}

DESTROY(std_Array_Stack){
	free(self->data);
return OK;
}
SET(std_Array_Stack){
	free(self->data);

	create(std_Array_Stack, self, 
		.typeSize = self->typeSize,
		.initSize = self->currSize,
		.data = value
	);
return OK;
}
HASH(std_Array_Stack){
	return hash_bytes(self->data, self->typeSize * self->currSize);
}
ITER(std_Array_Stack){
	if(index > self->currSize) index = self->currSize;

	return pntr_shiftcpy(self->data, self->currSize - index);
}


PRINT(std_Array_Stack){

	return write(out, 
	      "(ArrayStack){ ",
	       		".size = ", 	$(self->currSize), ", ",
			".alloced = ", 	$(self->allocSize),", ",
	       		".typeSize = ", $(self->typeSize), ", ",
	       		".data = ", 	$(self->data),
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

	self->data = calloc(alloc_size, arg.typeSize);

	self->allocSize = alloc_size;
	self->typeSize = arg.typeSize;

	if(arg.data){
		memcpy(self->data, arg.data, args->initSize * args->typeSize);
	  	
	  	self->currSize = arg.initSize;
	}

return self;


}


#include "../../../pkg.h"

import(std)

WRITE(std_Array_Buffer){
	nonull(self, return err;);
	
	if(size > this.items) size = this.items;

	loop(i, size)
		memcpy(pntr_shiftcpy(this.data, i * this.typeSize), data[i], this.typeSize);

return size;
}

READ(std_Array_Buffer){
	nonull(self, return err);

	if(0 == self->currSize) return ERR(DATAERR_EMPTY, "stack is empty");
	
	size = self->currSize < size ? self->currSize : size;

	loop(i, size){
		self->currSize--;
		memcpy(&((u8*)self->data)[i * self->typeSize], &((u8*)self->data)[self->currSize * self->typeSize], self->typeSize);
	}

return OK;
}

SIZE(std_Array_Buffer){
	if(!self)
		return sizeof(std_Array_Buffer);
	elif (elements)
	  	return self->currSize;
	else
	  	return self->currSize * self->typeSize;
}
COPY(std_Array_Buffer){

	std_Array_Buffer* dest = where;

	create(std_Array_Buffer, where, 
		.typeSize = self->typeSize,
		.initSize = self->currSize,
		.data = self->data
	);

return where;
}

DESTROY(std_Array_Buffer){
	free(self->data);
return OK;
}
SET(std_Array_Buffer){
	free(self->data);

	create(std_Array_Buffer, self, 
		.typeSize = self->typeSize,
		.initSize = self->currSize,
		.data = value
	);
return OK;
}
HASH(std_Array_Buffer){
	return hash_bytes(self->data, self->typeSize * self->currSize);
}
ITER(std_Array_Buffer){
	if(index > self->currSize) index = self->currSize;

	return pntr_shiftcpy(self->data, self->currSize - index);
}


PRINT(std_Array_Buffer){

	return write(out, 
	      "(ArrayBuffer){ ",
	       		".size = ", 	$(self->currSize), ", ",
			".alloced = ", 	$(self->allocSize),", ",
	       		".typeSize = ", $(self->typeSize), ", ",
	       		".data = ", 	$(self->data),
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


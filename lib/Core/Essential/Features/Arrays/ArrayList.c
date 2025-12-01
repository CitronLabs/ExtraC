#include "../../../../pkg.h"

import(std)


WRITE(std_ArrayList){

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

COPY(std_ArrayList){
	return create(std_ArrayList, where,  
		.initSize = self->currSize,
	       	.typeSize = self->typeSize,
	       	.data = self->data
	);
}

READ(std_ArrayList){

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

HASH(std_ArrayList){
	
	return hash_bytes(self->data, self->typeSize * self->currSize);
}

ITER(std_ArrayList){
	if(index > self->currSize) return null;

	return pntr_shiftcpy(self->data, (self->currSize + index) * self->typeSize);
}

SIZE(std_ArrayList){
	return elements ? self->currSize : self->currSize * self->typeSize;
}

SET(std_ArrayList){
	
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

DESTROY(std_ArrayList){

	free(self->data);

return OK;
}

PRINT(std_ArrayList){

	return write(out, 
	      "(ArrayList){ ",
	       	".size = ", 	$(self->currSize), ", ",
		".alloced = ", 	$(self->allocSize),", ",
	       	".typeSize = ", $(self->typeSize), ", ",
	       	".data = ", 	$(self->data),
	      " }", fmt_end);

}

construct(std_ArrayList,
FMT(),
DEF(),
	.Print   = std_ArrayList_Op_Print,
	.Create  = std_ArrayList_Op_Create,
	.Copy 	 = std_ArrayList_Op_Copy,
	.Size 	 = std_ArrayList_Op_Size,
	.Destroy = std_ArrayList_Op_Destroy,
	.Set	 = std_ArrayList_Op_Set,
	.Hash	 = std_ArrayList_Op_Hash,
	.Write   = std_ArrayList_Op_Write,
	.Read 	 = std_ArrayList_Op_Read,
	.Iter 	 = std_ArrayList_Op_Iter,
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



#include <Core/pkg.c>

#define module std, Memory

bool  moduleFn(compare)(void* a, void* b, len_t size);
void* moduleFn(setTo)(void* dest, int val, len_t size);
void* moduleFn(copyTo)(void* dest, void* from, len_t size);
void* moduleFn(heapAlloc)(len_t size);
void* moduleFn(grow)(void* data, len_t size);
void  moduleFn(heapDealloc)(void* data);


WRITE(std_Memory){
	
	len_t usage = 0, num_iter = 0;

	loop(i, size){
		std_Memory* mem = data[i];
		len_t copy_size = mem->size > this.size ? this.size : mem->size;

		memcpy(
		    pntr_shiftcpy(this.pointer, usage), 
		    mem->pointer,
		    copy_size
	 	);

		usage += copy_size;

		if(usage >= this.size){
			num_iter = i; break;
		}
	}

return num_iter;
}


READ(std_Memory){
	
	len_t usage = 0, num_iter = 0;

	loop(i, size){
		std_Memory* mem = data[i];
		len_t copy_size = mem->size > this.size ? mem->size : this.size;

		memcpy(
		    mem->pointer,
		    pntr_shiftcpy(this.pointer, usage), 
		    copy_size
	 	);

		usage += copy_size;

		if(usage >= this.size){
			num_iter = i; break;
		}
	}

return num_iter;
}

SET(std_Memory){
	word val = value ? *(word*)value : 0;

	if(this.pointer)
		memset(this.pointer, val, this.size);
	
return OK;
}

COPY(std_Memory){
	std_Memory* dest = where;

	if(!dest->__type)
		dest->__type = std_Memory_Type;

	if(!dest->pointer) {
		dest->pointer = malloc(this.size);
		dest->size = this.size;
	}


	memcpy(dest->pointer, 
		this.pointer, 
 		this.size > dest->size ? 
 			dest->size : this.size
 	);

return where;
}

DESTROY(std_Memory){

	if(this.pointer) free(this.pointer);

return OK;
}

HASH(std_Memory){
	return 
	    hash_bytes(&this.size, sizeof(len_t)) + 
	    hash_bytes(&this.pointer, sizeof(pntr))
	;
}

SIZE(std_Memory){ return this.size; }

PRINT(std_Memory){
	return printTo(out,
		"(std_Memory){ "
		    ".pointer = ", $(this.pointer), ", ",
		    ".size = ", $(this.size), ", ",
		" }"
	);
}

construct(std_Memory,
FMT(),
DEF(),
	.Create  = std_Memory_Op_Create,
	.Write 	 = std_Memory_Op_Write,
	.Copy 	 = std_Memory_Op_Copy,
	.Set 	 = std_Memory_Op_Set,
	.Size 	 = std_Memory_Op_Size,
	.Hash 	 = std_Memory_Op_Hash,
	.Read 	 = std_Memory_Op_Read,
	.Print 	 = std_Memory_Op_Print,
	.Destroy = std_Memory_Op_Destroy,
){
	this.pointer = malloc(arg.size);
	this.size = arg.size;

	if(!this.pointer){
		ERR(ERR.FAIL, "failed to allocate memory");
		return nil;
	}

return self;
}

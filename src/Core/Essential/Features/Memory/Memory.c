#include <XC.pkg.c>

#define module std, Memory

#undef memcmp
#undef memcpy
#undef memset

bool  moduleFn(compare)(void* a, void* b, len_t size){ return memcmp(a, b, size) == 0; }
void* moduleFn(setTo)(void* dest, int val, len_t size){ return memset(dest, val, size); }
void* moduleFn(copyTo)(void* dest, void* from, len_t size){ return memcpy(dest, from, size); }

std_Memory* moduleFn(getHeap)(){
	static std_Memory HeapMemory = {};

	if(HeapMemory.pointer == nil){
	    create(std_Memory, &HeapMemory, XC.Sys.Mem.getInfo().pageSize);

	    if(HeapMemory.pointer == nil){
		ERR(ERR.INIT, "Failed to initialize Heap memory");
		return nil;
	    }
	}

return &HeapMemory;
}

WRITE(std_Memory){
	len_t usage = 0, num_iter = 0;

	loop(i, size){
		std_Memory* mem = data[i];
		len_t copy_size = mem->pages > this.pages ? this.pages : mem->pages;

		memcpy(
		    pntr_shiftcpy(this.pointer, usage), 
		    mem->pointer,
		    copy_size
	 	);

		usage += copy_size;

		if(usage >= this.pages){
			num_iter = i; break;
		}
	}

return num_iter;
}


READ(std_Memory){
	
	len_t usage = 0, num_iter = 0;

	loop(i, size){
		std_Memory* mem = data[i];
		len_t copy_size = mem->pages > this.pages ? mem->pages : this.pages;

		memcpy(
		    mem->pointer,
		    pntr_shiftcpy(this.pointer, usage), 
		    copy_size
	 	);

		usage += copy_size;

		if(usage >= this.pages){
			num_iter = i; break;
		}
	}

return num_iter;
}

SET(std_Memory){
	word val = value ? *(word*)value : 0;

	if(this.pointer)
		memset(this.pointer, val, this.pages * XC.Sys.Mem.getInfo().pageSize);
	
return OK;
}

COPY(std_Memory){
	std_Memory* dest = where;

	if(!dest->__type)
		dest->__type = std_Memory_Type;

	if(!dest->pointer) {
		dest->pointer = malloc(this.pages);
		dest->pages = this.pages;
	}


	memcpy(dest->pointer, 
		this.pointer, 
 		XC.Sys.Mem.getInfo().pageSize *
		(this.pages > dest->pages ? 
 			dest->pages : this.pages)
 	);

return where;
}

DESTROY(std_Memory){

	if(this.pointer) free(this.pointer);

return OK;
}

HASH(std_Memory){
	return 
	    hash_bytes(&this.pages, sizeof(len_t)) + 
	    hash_bytes(&this.pointer, sizeof(pntr))
	;
}

SIZE(std_Memory){ return elements ? this.pages : this.pages * XC.Sys.Mem.getInfo().pageSize; }

PRINT(std_Memory){
	return printTo(out,
		"(std_Memory){ "
		    ".pointer = ", $(this.pointer), ", ",
		    ".pages = ",   $(this.pages),   ", ",
		" }"
	);
}

construct(std_Memory,
FMT(),
DEF(),
	.Create  = mod(Op_Create),
	.Write 	 = mod(Op_Write),
	.Copy 	 = mod(Op_Copy),
	.Set 	 = mod(Op_Set),
	.Size 	 = mod(Op_Size),
	.Hash 	 = mod(Op_Hash),
	.Read 	 = mod(Op_Read),
	.Print 	 = mod(Op_Print),
	.Destroy = mod(Op_Destroy),
){
	len_t pageSize = XC.Sys.Mem.getInfo().pageSize;
	this.pages     = arg.size / pageSize  + (pageSize % arg.size == 0 ? 0 : 1);
	this.pointer   = XC.Sys.Mem.alloc(this.pages);

	if(!this.pointer){
		ERR(ERR.FAIL, "failed to allocate memory");
		return nil;
	}

return self;
}

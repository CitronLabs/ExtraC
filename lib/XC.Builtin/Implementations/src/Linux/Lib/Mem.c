#include <XC/pkg.c>

#define module XC, Mem

from(env_Common_Posix, 
     use(Memory)
)

alias(env.Common.Posix, Posix)

void* moduleFn(alloc)(len_t num_pages){
	Memory mem = {0};

	create(Memory, &mem, 
		.size = num_pages * XC.Mem.getPageSize()
	);
	
	if(mem.address == nil){
		ERR(ERR.FAIL, "failed to allocate memory");
		return nil;
	}

	iferr(Posix.Memory.commit(&mem)){
		
	}

return mem.address;
}


errvt moduleFn(dealloc)(void* ptr, len_t num_pages){
	Memory mem = {
		.address = ptr,
		.userFlags = 0,
		.__private.size = num_pages * Posix.Memory.getPageSize()
	};

return pop(&mem);
}

len_t moduleFn(getPageSize)(){return Posix.Memory.getPageSize();}

errvt moduleFn(protect)(
	void* ptr, 
	len_t num_pages, 
	errvt fn(fault_callback)(void*, len_t), 
	word permissions
){
	Memory mem = {
		.address = ptr,
		.userFlags = 0,
		.__private.size = num_pages
	};
	
	if(permissions & XC.Mem.Perms.READ)  mem.__private.prot |= Posix.Memory.Flag.READ;
	if(permissions & XC.Mem.Perms.WRITE) mem.__private.prot |= Posix.Memory.Flag.WRITE;
	if(permissions & XC.Mem.Perms.EXEC)  mem.__private.prot |= Posix.Memory.Flag.EXECUTE;

return Posix.Memory.setProt(&mem, mem.__private.prot);
}


#include "../../pkg.h"
#include "../Buffer/pkg.h"

import(std)



errvt methodimpl(std_Arena, Reserve, u64 num_bytes){
	if(priv.alloc_size - priv.current_size > num_bytes) return OK;
	
	std.Arena.Grow(self, num_bytes);
 return OK;	
}
errvt methodimpl(std_Arena, Grow, u64 num_bytes){

	if(priv.isStatic){
		return ERR(MEMERR_OVERFLOW, "cannot grow a static arena");
	}
	if(priv.current_size + num_bytes > priv.max_size){
		return ERR(MEMERR_OVERFLOW, "size goes beyond the specified maximum");
	}
	u64 new_alloc_size = 
		((priv.alloc_size / 2) + num_bytes) > priv.max_size ?
		priv.max_size - priv.alloc_size : ((priv.alloc_size / 2) + num_bytes);

	std_Buffer* new_buff = new(std_Buffer, new_alloc_size, T(u8), true);
	write(priv.arena_buffers, &new_buff);

	priv.alloc_size += new_alloc_size;

 return OK;	
}
void* methodimpl(std_Arena, Alloc, u64 num_bytes){

	std_Buffer* alloc_buff = null;

	if(priv.current_size + num_bytes > priv.alloc_size){
		std.Arena.Grow(self, num_bytes);
	}else{
	    foreach(priv.arena_buffers, std_Buffer*, buff){
		if(std.Buffer.getMaxSize(*buff) != size(*buff) && 
		   std.Buffer.getMaxSize(*buff) >  size(*buff) + num_bytes)
		{ alloc_buff = *buff; break; }
    	    }
	}
	priv.current_size += num_bytes;

return std.Buffer.Allocator.New(generic alloc_buff, num_bytes);
}

void* imethodimpl(std_Arena, New, u64 size){ 
	self(std_Arena); 
return std.Arena.Alloc(self, size); 
}

void* imethodimpl(std_Arena, Resize, void* instance, u64 size){ 
	self(std_Arena); 
	std.Arena.Grow(self, size);
return instance;
}

errvt imethodimpl(std_Arena, setMax, u64 size){
	self(std_Arena);
	priv.max_size = size;
return OK;
}
u64 imethodimpl(std_Arena, getBytesAlloced){
	self(std_Arena);
return priv.alloc_size;
}

bool imethodimpl(std_Arena, isStatic){ 
	self(std_Arena);
return priv.isStatic;
}

COPY(std_Arena){
	nonull(self || where, return nil);

	if(create(std_Arena, where, false, 
		.isStatic = priv.isStatic,
		.init_size = priv.current_size
	) == nil)
		{ERR(ERR_FAIL, "failed to create copy"); return nil; }

	foreach(priv.arena_buffers, std_Buffer*, buff){
		pntr copy_loc = std.Arena.Alloc(where, size(*buff));

		memcpy(copy_loc, std.Buffer.getPointer(*buff), size(*buff));
	}

return OK;
}

SIZE(std_Arena){
return priv.current_size;
}

SET(std_Arena){
	nonull(value, return err);

	priv.max_size = *(len_t*)value;

return OK;
}

DESTROY(std_Arena){
	foreach(priv.arena_buffers, std_Buffer*, buff){
		del(buff);
	}
return OK;
}

PRINT(std_Arena){
	nonull(self || out, return 0);

	return write(out, 
		"(Arena){ "
	 	    ".in_use = ",   $(priv.current_size), ", ",
		    ".alloced = ",  $(priv.alloc_size), ", ",
	 	    ".max_size = ", $(priv.max_size), ", ",
	 	    ".static = ",   $(priv.isStatic),
	 	" }",
	fmt_end);
}

construct(std_Arena,
FMT(),
DEF(),
	.Write   = nilmethod,
	.Read    = nilmethod,
	.Hash	 = nilmethod,
	.Iter	 = nilmethod,
	.Scan 	 = nilmethod,
	.Create  = std_Arena_Op_Create,
	.Destroy = std_Arena_Op_Destroy,
	.Set	 = std_Arena_Op_Set,
	.Copy	 = std_Arena_Op_Copy,
	.Size	 = std_Arena_Op_Size,
	.Print 	 = std_Arena_Op_Print
){
	if(arg.init_size == 0) {
	    ERR(MEMERR_INVALIDSIZE, "initial size cannot be 0 for priv");
	    return nil;
	}

	priv.max_size 	   = maxof(u64);
	priv.arena_buffers = newArrayList(std_Buffer*, 5);
	priv.alloc_size    = args->init_size;
	priv.current_size  = 0;
	
return self;
}

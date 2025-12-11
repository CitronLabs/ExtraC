#include <Core/pkg.c>
#define module std, Pool


void* moduleMethod(std_Pool, Alloc, u64 num){

	std_Buffer* alloc_buff = nil;

	if(priv.current_size + num > priv.alloc_size){

	}else{
	    foreach(priv.pool_buffers, std_Buffer*, buff){
		if(std.Buffer.getMaxSize(*buff) != size(*buff) && 
		   std.Buffer.getMaxSize(*buff) >  size(*buff) + num)
		{
			alloc_buff = *buff;
			break;
		}
    	    }
	}
	priv.current_size += num;

return std.Buffer.Allocator.New(generic alloc_buff, num);
}

void* moduleIMethod(std_Pool, New, len_t size){
	self(std_Pool);
return std.Pool.Alloc(self, size);
}

errvt moduleMethod(std_Pool, Return, void* instance){
	write(priv.free_slots, instance);
return OK;
}


errvt moduleIMethod(std_Pool, Delete, void* instance){	
	self(std_Pool);
	write(priv.free_slots, instance);
return OK;
}

errvt moduleMethod(std_Pool, Grow, u64 num){

	if(priv.isStatic){
		return ERR(ERR.MEM.OVERFLOW, "cannot grow a static pool");
	}
	if(priv.current_size + num > priv.max_size){
		return ERR(ERR.MEM.OVERFLOW, "size goes beyond the specified maximum");
	}
	u64 new_alloc_size = 
		((priv.alloc_size / 2) + num) > priv.max_size ?
		priv.max_size - priv.alloc_size : ((priv.alloc_size / 2) + num);

	std_Buffer* new_buff = new(std_Buffer, new_alloc_size, priv.type);
	write(priv.pool_buffers, &new_buff);

	priv.alloc_size += new_alloc_size;

return OK;
}

errvt moduleMethod(std_Pool, Reserve, u64 num){
	if(priv.alloc_size - priv.current_size > num) return OK;

	std.Pool.Grow(self, num);
return OK;}


errvt moduleMethod(std_Pool, ForceDestroy){
	priv.num_in_use = 0;
	ops(std_Pool_Type).Destroy(self);

return OK;
}

errvt moduleIMethod(std_Pool, setMax, u64 size){
	self(std_Pool);
	nonull(self){ return err; }

	priv.max_size = size;
return OK;
}
bool  moduleIMethod(std_Pool, isStatic){ 
	self(std_Pool);
	nonull(self){ return err; }

return priv.isStatic;
}
u64   moduleIMethod(std_Pool, getBytesAlloced){
	self(std_Pool);
	nonull(self){ return err; }

return priv.alloc_size * priv.type.size;
}

SIZE(std_Pool){
	if(elements)
		return priv.current_size;
	else
		return priv.current_size * priv.type.size;
}

SET(std_Pool){
	nonull(self, value){ return err; }

	priv.max_size = *(len_t*)value;

return OK;
}

COPY(std_Pool){
	nonull(self, where){ return nil; }

	if(create(std_Pool, where,  
		.isStatic = priv.isStatic,
		.init_size = priv.current_size
	) == nil)
		{ ERR(ERR.FAIL, "failed to create copy"); return nil; }

	foreach(priv.pool_buffers, std_Buffer*, buff){
		pntr copy_loc = std.Pool.Alloc(where, size(*buff));

		memcpy(copy_loc, std.Buffer.getPointer(*buff), size(*buff));
	}

return where;
}

DESTROY(std_Pool){
	nonull(self){ return err; }

	if(priv.num_in_use != 0) 
		return ERR(ERR.FAIL, "not all object have been returned to the pool yet");

	foreach(priv.pool_buffers, std_Buffer*, buff){
	    del(buff);
	}
	
	del(priv.pool_buffers, priv.free_slots);

return OK;
}

ITER(std_Pool){
	if(index > priv.current_size){
		ERR(ERR.FAIL, "index out of range");
		return nil; 
	}

	foreach(priv.pool_buffers, std_Buffer*, buff){
	    if(index < elements(*buff)){
		return index(*buff, index);
	    }
	}

	ERR(ERR.FAIL, "failed to index");

return nil;
}
WRITE(std_Pool){
	nonull(self, data){ return 0; }

	loop(i, size){
	    if(data[i])
		std.Pool.Return(self, data[i]);
	}
return size;
}
READ(std_Pool){
	nonull(self, data){ return 0; }

	if(priv.current_size + size > priv.max_size)
		size = priv.max_size - priv.current_size;

	loop(i, size){
	    if(data[i])
		*((pntr*)data[i]) = std.Pool.Alloc(self, 1);
	}

return size;

}
PRINT(std_Pool){
	nonull(self, out){ return 0; }

	return write(out, 
		"(std_Pool){ "
	 	    ".in_use = ", $(priv.current_size), ", ",
		    ".alloced = ", $(priv.alloc_size), ", ",
	 	    ".max_size = ", $(priv.max_size), ", ",
	 	    ".static = ", $(priv.isStatic),
	 	" }",
	fmt_end);

}

construct(std_Pool,
FMT(),
DEF(),
	.Hash	 = nil,
	.Scan 	 = nil,
	.Create  = std_Pool_Op_Create,
	.Destroy = std_Pool_Op_Destroy,
	.Set	 = std_Pool_Op_Set,
	.Copy	 = std_Pool_Op_Copy,
	.Size	 = std_Pool_Op_Size,
	.Print 	 = std_Pool_Op_Print,
	.Write   = std_Pool_Op_Write,
	.Read 	 = std_Pool_Op_Read,
	.Iter	 = std_Pool_Op_Iter,
){

	if(0 == arg.type.size){
		ERR(ERR.INVALID, "required type size cannot be 0");
	  	return nil;
	}
	
	if(0 == arg.init_size){ 
	      	ERR(ERR.INVALID, "required init size cannot be 0");
	  	return nil;
	}

	arg.limit = args->limit == 0 ? maxof(u64) : args->limit;
	
	if(arg.init_size > args->limit) {
	  	ERR(ERR.INVALID, "init size cannot be larger than the limit");
	  	return nil;
	}
	
	
	priv.max_size = arg.limit;
	priv.current_size = arg.init_size;
	priv.type = arg.type;
	

return self;
}


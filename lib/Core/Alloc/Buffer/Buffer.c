#include "../../pkg.h"

import(std)


u64  		methodimpl (std_Buffer, getMaxItems)	{ return priv.alloced_size; }
std_typeData	methodimpl (std_Buffer, getType)	{ return priv.type; }
u64  		methodimpl (std_Buffer, getMaxSize)	{ return priv.type.size * priv.alloced_size; }
pntr		methodimpl (std_Buffer, getPointer)	{ return priv.data; }
u64 		imethodimpl(std_Buffer, getBytesAlloced){ self(std_Buffer); return priv.type.size * priv.size; }
bool		imethodimpl(std_Buffer, isStatic)	{ self(std_Buffer); return priv.isStatic; }

errvt imethodimpl(std_Buffer, setMax, u64 max){ 
	self(std_Buffer); 
	return std.Buffer.resize(self, max); 
}

pntr imethodimpl(std_Buffer, New, u64 size){
	self(std_Buffer);
	if(priv.alloced_size + size > priv.size){
		if(priv.isStatic){
			ERR(MEMERR_OVERFLOW, "size exceeds buffer and cannot grow static buffer");
			return null;
		}else{
			iferr(std.Buffer.resize(self, (priv.size / 2) + size))
				return null;	
		}
	}

	pntr result = pntr_shiftcpy(priv.data, priv.alloced_size);
	priv.alloced_size += size;

return result;
}

void* imethodimpl(std_Buffer, Realloc, pntr instance, u64 new_size){
	self(std_Buffer);
	iferr(std.Buffer.resize(self, new_size)){
		return null;
	}
return priv.data;
}

errvt methodimpl(std_Buffer, Cast, std_typeData type){
	if(type.size == 0) return ERR(MEMERR_INVALIDSIZE, "cannot cast buffer to type size 0");
	
	priv.size = (priv.size * priv.type.size) / type.size;
	priv.alloced_size = (priv.alloced_size * priv.type.size) / type.size;
	priv.type = type;

return OK;
}
errvt methodimpl(std_Buffer, Resize, u64 new_size){
	if(priv.isStatic) 
		return ERR(MEMERR_INITFAIL, "unable to resize a static buffer");
	priv.data = realloc(priv.data, new_size);
	priv.size = new_size;
	if(priv.alloced_size > priv.size) priv.alloced_size = priv.size;

return OK;
}

std_Buffer* methodimpl(std_Buffer, fromView, void* start, std_typeData type, u64 len){
	nonull(start || self, return nil);
	
	if(priv.data) del(self);

	priv.data = start;
	priv.size = len;
	priv.alloced_size = len;
	priv.type = type;
	priv.isStatic = true;
	

return self;
}


HASH(std_Buffer){
	return hash_bytes(priv.data, priv.size);
}
DESTROY(std_Buffer){
	free(priv.data);
return OK;
}

SIZE(std_Buffer){
	if(elements)
	    return priv.size;
	else
	    return priv.size * priv.type.size;
}

SET(std_Buffer){
	nonull(self, return err);

	if(value == null) 
		memset(priv.data, 0, priv.size * priv.type.size);
	else 
		memcpy(priv.data, value, priv.size * priv.type.size);	
	
return OK;
}

ITER(std_Buffer){
	nonull(self, return null);

	if(index >= priv.size){ 
		ERR(ERR_INVALID, "index out of range");
		return null; 
	}

return pntr_shiftcpy(priv.data, priv.type.size * index);
}

COPY(std_Buffer){
	nonull(self || where, return nil);

	if(create(std_Buffer, where, 
		.type 	  = priv.type,
		.size 	  = priv.size,
		.isStatic = false,
		.initData = priv.data
	) == nil)
		{ERR(ERR_FAIL, "failed to create copy"); return nil;}

	void* copy_loc = std.Buffer.Allocator.New(where, priv.size);

	memcpy(copy_loc, priv.data, priv.type.size * priv.size);

return where;
}

READ(std_Buffer){
	nonull(self || data, return 0);

	if(size > priv.size) size = priv.size;

	loop(i, size){
		data[i] = pntr_shiftcpy(priv.data, i * priv.type.size);
	}

return size;
}

WRITE(std_Buffer){
	nonull(self || data, return 0);

	if((size + priv.size) > priv.alloced_size) size = priv.alloced_size - priv.size;

	loop(i, size){
		void* copy_loc = pntr_shiftcpy(priv.data, (i + priv.size) * priv.type.size);

		if(priv.type.ops->Copy != nilmethod){
		    priv.type.ops->Copy(data[i], copy_loc);
		}else{
		    memcpy(copy_loc, data[i], priv.type.size);
		}
	}

return size;
}

PRINT(std_Buffer){
	nonull(self || out, return 0);

	u64 formated_len = 0;

	formated_len += write(out, "(std_Buffer){ ",
		 	".items = ", $(priv.size), ", ",
			".limit = ", $(priv.alloced_size), ", ",
		 	".type  = ", $use(std_typeData_Type, &priv.type), ", ", 
		     	".data  = { ",
	fmt_end);

	loop(i, priv.size)
		formated_len += write(out, 
		  	$use(&priv.type, 
   			     pntr_shiftcpy(priv.data, i + priv.type.size)
   			), ", ",
		fmt_end);

	formated_len += write(out, " }\n}", fmt_end);

return formated_len;
}


construct(std_Buffer,
FMT(),
DEF(),
	.Create  = std_Buffer_Op_Create,
	.Destroy = std_Buffer_Op_Destroy,
	.Size	 = std_Buffer_Op_Size,
	.Iter	 = std_Buffer_Op_Iter,
	.Copy	 = std_Buffer_Op_Copy,
	.Write   = std_Buffer_Op_Write,
	.Read	 = std_Buffer_Op_Read,
	.Hash	 = std_Buffer_Op_Hash,
	.Print	 = std_Buffer_Op_Print,
	.Set	 = std_Buffer_Op_Set
){
	if(args->size == 0){
	  	ERR(MEMERR_INVALIDSIZE, "buffer size cannot be 0"); 
	  	return nil;
	}

	args->type.size = args->type.size == 0 ? 1 : args->type.size;
	arg.type.ops = args->type.ops;

	if(args->isStatic){
		priv.data = pntr_shiftcpy(self, sizeof(std_Buffer));
	}else{
	  	priv.data = calloc(args->size, args->type.size);

		if(!priv.data)
			return nil;
	}

	priv.size = args->size;
	priv.type = arg.type;
	priv.isStatic = arg.isStatic;
	priv.alloced_size = 0;

return self;
}



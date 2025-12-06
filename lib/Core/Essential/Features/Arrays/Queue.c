#define module std, Array, Queue
#include "includes.h"

#define write_queue(in, index)								\
	memcpy(priv.writehead, &((u8*)in)[index * this.typeSize], this.typeSize);	\
	pntr_shift(priv.writehead,this.typeSize);					\
	this.items++;									

#define read_queue(out, index)								\
	memcpy(&((u8*)out)[index * this.typeSize], priv.readhead, this.typeSize);	\
	pntr_shift(priv.readhead,this.typeSize);					\
	this.items--;	


#define get_slot_dist(a, b) ((pntr_asVal(b) - pntr_asVal(a)) / slot_size)

#define slot_size (this.typeSize)



errvt std_Array_Queue_Grow(Queue* self, u64 add_amount){
	
	u64 queue_allocsize = get_slot_dist(priv.start, priv.end); 
	
	if(priv.writehead < priv.readhead && priv.jmp_point == nil){
		
		u64 slots_btwn_wrt_rd = get_slot_dist(priv.writehead, priv.readhead);

		priv.jmp_point = priv.end; 
	}
	
	void* new_buf = realloc(priv.start, get_slot_dist(priv.start, priv.end) + add_amount);

	if(priv.jmp_point != nil)
		priv.jmp_point = new_buf + (get_slot_dist(priv.start, priv.jmp_point) * slot_size);
		
	priv.end = new_buf + (get_slot_dist(priv.start, priv.end) * slot_size);
	priv.readhead = new_buf + (get_slot_dist(priv.start, priv.readhead)  * slot_size);
	priv.writehead = new_buf + (get_slot_dist(priv.start, priv.writehead)  * slot_size);
	

return OK;
}

void*  moduleMethod(std_Array_Queue, ToPointer){
	nonull(self){ return nil; }

	len_t new_buff_size 	= pntr_dist(priv.start, priv.end);
	void* new_buff 	    	= malloc(new_buff_size);

	len_t items_num 	= this.items;
	
	std.Array.Read(self, 
			new_buff, 
			this.items
	);

	free(priv.start);

	priv.start 		= new_buff;
	priv.end   		= pntr_shiftcpy(new_buff, new_buff_size);
	priv.jmp_point 		= nil;
	priv.items_til_jump 	= 0;
	priv.readhead 		= priv.start;
	priv.writehead 		= pntr_shiftcpy(priv.readhead, this.typeSize * items_num);
	this.items 		= items_num;
	
return priv.start;
}
errvt  moduleMethod(std_Array_Queue, Reserve, bool exact, u64 amount){
	nonull(self){ return err; }

	errvt result = OK;

	if(exact)
		result = std_Array_Queue_Grow
			(self, amount);
	else
		result = std_Array_Queue_Grow
			(self, (pntr_dist(priv.start, priv.end) / 2) + amount);

return result;

}
noFail moduleMethod(std_Array_Queue, Clear){
	nonull(self, return);

	len_t new_buff_size 	= pntr_dist(priv.start, priv.end);
	void* new_buff 	    	= malloc(new_buff_size);

	free(priv.start);

	priv.start 		= new_buff;
	priv.end   		= pntr_shiftcpy(new_buff, new_buff_size);
	priv.jmp_point 		= nil;
	priv.items_til_jump 	= 0;
	priv.readhead 		= priv.start;
	priv.writehead 		= priv.start;
	this.items 		= 0;
}


WRITE(std_Array_Queue){
	nonull(self){ return err; }

	u64 queue_allocsize = get_slot_dist(priv.start, priv.end);

	if(this.items + size > queue_allocsize){
		std_Array_Queue_Grow(self, (queue_allocsize / 2) + size);
	}

	loop(i, size){
		write_queue(data[i], i);
	}

return size;
}

READ(std_Array_Queue){
	nonull(self){ return err; }

	if(size > this.items)
		size = this.items;

	loop(i, size){
		if(priv.readhead == priv.jmp_point)
			priv.readhead = priv.start;
	
		if(priv.items_til_jump == 0){
			read_queue(data[i], i);
			priv.readhead = priv.jmp_point;					
		}else{
			read_queue(data[i], i);
		}

		if(priv.items_til_jump > 0) priv.items_til_jump--;
	}

return size;
}

SIZE(std_Array_Queue){ return self ? elements ? this.items : this.items * this.typeSize : sizeof(std_Array_Queue); }

COPY(std_Array_Queue){
	
	std_Object* dest_obj = where;

	switchT(dest_obj->__type){
	caseT(List){
	    List* dest = where;

	    if(dest->typeSize != this.typeSize){
		ERR(ERR_INVALID, "type sizes dont match between copying arrays");
		return nil;
	    }

	    
	    if(dest->items + this.items > privof(dest).allocSize)
			std_Array_List_Grow(dest, (privof(dest).allocSize / 2) + this.items);

	    loop(i, this.items)
	    	write(dest, index(self, i));
	break;}
	caseT(Buffer){
	    Buffer* dest = where;

	    if(dest->typeSize != this.typeSize){
		ERR(ERR_INVALID, "type sizes dont match between copying arrays");
		return nil;
	    }

	    loop(i, this.items)
	    	memcpy(
		    pntr_shiftcpy(dest->data, i * this.typeSize), 
		    index(self, i), 
		    this.typeSize
		);

	break;}
	caseT(Stack){
	    Stack* dest = where;

	    if(dest->typeSize != this.typeSize){
		ERR(ERR_INVALID, "type sizes dont match between copying arrays");
		return nil;
	    }

	    if(dest->items + this.items > privof(dest).allocSize)
			std_Array_Stack_Grow(dest, (privof(dest).allocSize / 2) + this.items);

	    loop(i, this.items)
	    	write(dest, index(self, i));
	break;}
	caseT(Queue){
	    Queue* dest = where;

	    if(dest->typeSize != this.typeSize){
		ERR(ERR_INVALID, "type sizes dont match between copying arrays");
		return nil;
	    }

	    len_t alloc_size = (pntr_asVal(privof(dest).end) - pntr_asVal(privof(dest).start)) / this.typeSize;
	    
	    if(dest->items + this.items > alloc_size)
			std_Array_Queue_Grow(dest, (alloc_size / 2) + this.items);
	    
	    loop(i, this.items)
		write(dest, index(self, i));
	    
	break;}
	defaultT{
		ERR(ERR_INVALID, "invalid copy destination type detected");
		return nil;
	}
	}

	create(List, where,  
		.initSize = this.items,
	       	.typeSize = this.typeSize,
	);

	loop(i, this.items){
		write((Queue*)where, index(self, i));
	}

return where;
}

DESTROY(std_Array_Queue){
	free(priv.start);
return OK;
}
SET(std_Array_Queue){
	free(priv.start);
	
	create(std_Array_Queue, self,  
		.typeSize = this.typeSize,
		.initSize = this.items,
		.data = value
	);

return OK;
}


errvt std_Array_Queue_Op_Get(std_Array_Queue* self, void* buff){

	u8* readhead = priv.readhead;
	loop(i, this.items){
		if(priv.readhead == priv.jmp_point)
			priv.readhead = priv.start;
	
		if(priv.items_til_jump == 0){
			read_queue(pntr_shiftcpy(buff, i * this.typeSize), i);
			priv.readhead = priv.jmp_point;					
		}else{
			read_queue(pntr_shiftcpy(buff, i * this.typeSize), i);
		}

		if(priv.items_til_jump > 0) priv.items_til_jump--;
	}

return OK;
}

HASH(std_Array_Queue){
	void* temp_buff = malloc(this.typeSize * this.items);

	std_Array_Queue_Op_Get(self, temp_buff);

	u32 result = hash_bytes(temp_buff, this.typeSize * this.items);
	
	free(temp_buff);

return result;
}

ITER(std_Array_Queue){
	nonull(self){ return nil; }

	if(this.items <= index) {
		ERR(DATAERR_OUTOFRANGE, "index exceeds queue");
		return nil;
	}

	void* index_start = priv.readhead;

	if(priv.jmp_point != nil && get_slot_dist(priv.readhead, priv.jmp_point) < index){
		index -= get_slot_dist(priv.readhead, priv.jmp_point);
		index_start = priv.start;

		if(priv.writehead > priv.readhead){
			index -= this.items - get_slot_dist(priv.readhead, priv.writehead);
			index_start = priv.jmp_point;
		}
	}
	else if(get_slot_dist(priv.readhead, priv.end) < index){
		index -= get_slot_dist(priv.readhead, priv.end);
		index_start = priv.start;
	}

return pntr_shiftcpy(index_start, index * this.typeSize);
}


PRINT(std_Array_Queue){
	return write(out, 
	      "(ArrayQueue){ ",
	       	".size = ", 		$(this.items), 	", ",
	       	".typeSize = ", 	$(this.typeSize),	", ",
	       	".start = ", 		$(priv.start), 	", ",
	       	".end = ", 		$(priv.end), 		", ",
	       	".writehead = ",	$(priv.writehead), 	", ",
	       	".readhead = ", 	$(priv.readhead), 	", ",
	      	".jmp_point = ",	$(priv.jmp_point),	", ",
	      	".items_til_jump = ",	$(priv.items_til_jump),", ",
	      " }", fmt_end);
}
construct(std_Array_Queue,
FMT(),
DEF(),
	.Print   = std_Array_Queue_Op_Print,
	.Create  = std_Array_Queue_Op_Create,
	.Copy 	 = std_Array_Queue_Op_Copy,
	.Size 	 = std_Array_Queue_Op_Size,
	.Destroy = std_Array_Queue_Op_Destroy,
	.Set	 = std_Array_Queue_Op_Set,
	.Hash	 = std_Array_Queue_Op_Hash,
	.Write   = std_Array_Queue_Op_Write,
	.Read 	 = std_Array_Queue_Op_Read,
	.Iter 	 = std_Array_Queue_Op_Iter,
	.Scan    = nilmethod,
){
	u64 start_size = arg.initSize == 0 ? 10 : args->initSize;

	*self = (std_Array_Queue){
		.__private.start = calloc(start_size, arg.typeSize),
		.typeSize 	 = arg.typeSize,
		.items 		 = 0,
	};
	
	priv.end = &(((u8*)priv.start)[start_size * arg.typeSize]);
	priv.readhead  = priv.start;
	priv.writehead = priv.start;

	if(arg.data != 0 && args->initSize != 0){
		loop(i, start_size){
			write_queue(arg.data, i);
		}
	}

return self;

}


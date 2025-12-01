#include "../../../../pkg.h"

import(std)


#define write_queue(in, index)								\
	memcpy(self->writehead, &((u8*)in)[index * self->typeSize], self->typeSize);	\
	pntr_shift(self->writehead,self->typeSize);					\
	self->items++;									

#define read_queue(out, index)								\
	memcpy(&((u8*)out)[index * self->typeSize], self->readhead, self->typeSize);	\
	pntr_shift(self->readhead,self->typeSize);					\
	self->items--;	


#define get_slot_dist(a, b) ((pntr_asVal(b) - pntr_asVal(a)) / slot_size)

#define slot_size (self->typeSize)


errvt methodimpl(std_ArrayQueue, Grow, u64 add_amount){
	
	u64 queue_allocsize = get_slot_dist(self->start, self->end); 
	
	if(self->writehead < self->readhead && self->jmp_point == null){
		
		u64 slots_btwn_wrt_rd = get_slot_dist(self->writehead, self->readhead);

		self->jmp_point = self->end; 
	}
	
	void* new_buf = realloc(self->start, get_slot_dist(self->start, self->end) + add_amount);

	if(self->jmp_point != null)
		self->jmp_point = new_buf + (get_slot_dist(self->start, self->jmp_point) * slot_size);
		
	self->end = new_buf + (get_slot_dist(self->start, self->end) * slot_size);
	self->readhead = new_buf + (get_slot_dist(self->start, self->readhead)  * slot_size);
	self->writehead = new_buf + (get_slot_dist(self->start, self->writehead)  * slot_size);
	

return OK;
}



WRITE(std_ArrayQueue){
	nonull(self, return err);

	u64 queue_allocsize = get_slot_dist(self->start, self->end);

	if(self->items + size > queue_allocsize){
		std_ArrayQueue_Grow(self, (queue_allocsize / 2) + size);
	}

	loop(i, size){
		write_queue(data[i], i);
	}

return size;
}

READ(std_ArrayQueue){
	nonull(self, return err);

	if(size > self->items)
		size = self->items;

	loop(i, size){
		if(self->readhead == self->jmp_point)
			self->readhead = self->start;
	
		if(self->items_til_jump == 0){
			read_queue(data[i], i);
			self->readhead = self->jmp_point;					
		}else{
			read_queue(data[i], i);
		}

		if(self->items_til_jump > 0) self->items_til_jump--;
	}

return size;
}

SIZE(std_ArrayQueue){ return self ? elements ? self->items : self->items * self->typeSize : sizeof(std_ArrayQueue); }
COPY(std_ArrayQueue){
	
	std_ArrayQueue* dest = where;

	create(std_ArrayQueue, dest, 
		.typeSize = self->typeSize,
		.initSize = self->items
	);

	loop(i, self->items){
		write(dest, index(self, i));
	}

return where;
}

DESTROY(std_ArrayQueue){
	free(self->start);
return OK;
}
SET(std_ArrayQueue){
	free(self->start);
	
	create(std_ArrayQueue, self,  
		.typeSize = self->typeSize,
		.initSize = self->items,
		.data = value
	);

return OK;
}


errvt std_ArrayQueue_Op_Get(std_ArrayQueue* self, void* buff){

	u8* readhead = self->readhead;
	loop(i, self->items){
		if(self->readhead == self->jmp_point)
			self->readhead = self->start;
	
		if(self->items_til_jump == 0){
			read_queue(pntr_shiftcpy(buff, i * self->typeSize), i);
			self->readhead = self->jmp_point;					
		}else{
			read_queue(pntr_shiftcpy(buff, i * self->typeSize), i);
		}

		if(self->items_til_jump > 0) self->items_til_jump--;
	}

return OK;
}

HASH(std_ArrayQueue){
	void* temp_buff = malloc(self->typeSize * self->items);

	std_ArrayQueue_Op_Get(self, temp_buff);

	u32 result = hash_bytes(temp_buff, self->typeSize * self->items);
	
	free(temp_buff);

return result;
}

ITER(std_ArrayQueue){
	nonull(self, return null);

	if(self->items <= index) {
		ERR(DATAERR_OUTOFRANGE, "index exceeds queue");
		return null;
	}

	void* index_start = self->readhead;

	if(self->jmp_point != null && get_slot_dist(self->readhead, self->jmp_point) < index){
		index -= get_slot_dist(self->readhead, self->jmp_point);
		index_start = self->start;

		if(self->writehead > self->readhead){
			index -= self->items - get_slot_dist(self->readhead, self->writehead);
			index_start = self->jmp_point;
		}
	}
	else if(get_slot_dist(self->readhead, self->end) < index){
		index -= get_slot_dist(self->readhead, self->end);
		index_start = self->start;
	}

return pntr_shiftcpy(index_start, index * self->typeSize);
}


PRINT(std_ArrayQueue){
	return write(out, 
	      "(ArrayQueue){ ",
	       	".size = ", 		$(self->items), 	", ",
	       	".typeSize = ", 	$(self->typeSize),	", ",
	       	".start = ", 		$(self->start), 	", ",
	       	".end = ", 		$(self->end), 		", ",
	       	".writehead = ",	$(self->writehead), 	", ",
	       	".readhead = ", 	$(self->readhead), 	", ",
	      	".jmp_point = ",	$(self->jmp_point),	", ",
	      	".items_til_jump = ",	$(self->items_til_jump),", ",
	      " }", fmt_end);
}
construct(std_ArrayQueue,
FMT(),
DEF(),
	.Print   = std_ArrayQueue_Op_Print,
	.Create  = std_ArrayQueue_Op_Create,
	.Copy 	 = std_ArrayQueue_Op_Copy,
	.Size 	 = std_ArrayQueue_Op_Size,
	.Destroy = std_ArrayQueue_Op_Destroy,
	.Set	 = std_ArrayQueue_Op_Set,
	.Hash	 = std_ArrayQueue_Op_Hash,
	.Write   = std_ArrayQueue_Op_Write,
	.Read 	 = std_ArrayQueue_Op_Read,
	.Iter 	 = std_ArrayQueue_Op_Iter,
	.Scan    = nilmethod,
){
	u64 start_size = arg.initSize == 0 ? 10 : args->initSize;

	*self = (std_ArrayQueue){
		.start 	  = calloc(start_size, arg.typeSize),
		.typeSize = arg.typeSize,
		.items = 0,
	};
	
	self->end = &(((u8*)self->start)[start_size * arg.typeSize]);
	self->readhead  = self->start;
	self->writehead = self->start;

	if(arg.data != null && args->initSize != 0){
		loop(i, start_size){
			write_queue(arg.data, i);
		}
	}

return self;

}


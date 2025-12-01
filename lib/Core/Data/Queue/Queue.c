#include "../../pkg.h"
#include "../DSN/pkg.h"
#include "../config.c"

import(std)


#define EMPTY 0
#define FULL 1
#define JUMP 2

#define write_queue(in, index)								\
	*priv.writehead = FULL;							\
	pntr_shift(priv.writehead, sizeof(slot_status));				\
	memcpy(priv.writehead, &((u8*)in)[index * priv.type.size], priv.type.size);	\
	pntr_shift(priv.writehead,priv.type.size);					\
	priv.items++;									

#define read_queue(out, index)								\
	*priv.readhead = EMPTY;							\
	pntr_shift(priv.readhead, sizeof(slot_status));				\
	memcpy(&((u8*)out)[index * priv.type.size], priv.readhead, priv.type.size);	\
	pntr_shift(priv.readhead,priv.type.size);					\
	priv.items--;	


#define get_slot_dist(a, b) ((pntr_asVal(b) - pntr_asVal(a)) / slot_size)

#define slot_size (priv.type.size + sizeof(slot_status))



typedef u8 slot_status;

errvt methodimpl(std_Queue, Grow, u64 add_amount){
	
	u64 queue_allocsize = get_slot_dist(priv.start, priv.end); 
	
	if(queue_allocsize + add_amount > priv.limit)
		return ERR(DATAERR_LIMIT, "cannot grow queue past limit");

	if(priv.writehead < priv.readhead && priv.jmp_point == null){
		
		u64 slots_btwn_wrt_rd = get_slot_dist(priv.writehead, priv.readhead);

		*(&(((u8*)priv.writehead)[(slots_btwn_wrt_rd - 1)* slot_size])) = JUMP;

		priv.jmp_point = priv.end; 
	
	}
	
	void* new_buf = realloc(priv.start, get_slot_dist(priv.start, priv.end) + add_amount);

	if(priv.jmp_point != null)
		priv.jmp_point = new_buf + (get_slot_dist(priv.start, priv.jmp_point) * slot_size);
		
	priv.end = new_buf + (get_slot_dist(priv.start, priv.end) * slot_size);
	priv.readhead = new_buf + (get_slot_dist(priv.start, priv.readhead)  * slot_size);
	priv.writehead = new_buf + (get_slot_dist(priv.start, priv.writehead)  * slot_size);
	

return OK;
}

errvt methodimpl(std_Queue, Reserve, bool exact, u64 amount){
	nonull(self, return err);

	errvt result = OK;

	if(exact)
		result = std_Queue_Grow(self, amount);
	else
		result = std_Queue_Grow(self, priv.items + (priv.items / 2) + amount);

return result;
}

errvt methodimpl(std_Queue, Enqueue, void* item, u64 num){
	nonull(self, return err);

	u64 queue_allocsize = get_slot_dist(priv.start, priv.end);

	if(priv.items + num > queue_allocsize){
		if(__Queue.autoGrow)
			std_Queue_Grow(self, (queue_allocsize / 2) + num);
		else
			return ERR(DATAERR_OUTOFRANGE, "grow the queue to fit new data");
	}

	loop(i, num){
		if(*priv.writehead == JUMP){
			pntr_shift(priv.writehead, sizeof(slot_status));				
			memcpy(priv.writehead, &((u8*)item)[i * priv.type.size], priv.type.size);	
			priv.items++;									
		}
		else{
			write_queue(item, i);
		}
	}

return OK;
}
errvt methodimpl(std_Queue, Dequeue, void* out, u64 num){
	nonull(self, return err);

	if(num > priv.items)
		return ERR(DATAERR_OUTOFRANGE, "num exceeds queue");

	loop(i, num){
		if(priv.readhead == priv.jmp_point)
			priv.readhead = priv.start;
	
		if(*priv.readhead == JUMP){
			read_queue(out, i);
			priv.readhead = priv.jmp_point;					
		}else{
			read_queue(out, i);
		}
	}

return OK;
}
bool methodimpl(std_Queue, Check){
	nonull(self, return false);

return priv.items != 0;	
}

u64 methodimpl(std_Queue, Count){
	nonull(self, return 0);

return priv.items;	
}

errvt methodimpl(std_Queue, Index, bool write, u64 index, void* data){
	nonull(self, return err);

	if(priv.items <= index) 
		return ERR(DATAERR_OUTOFRANGE, "index exceeds queue");

	void* index_start = priv.readhead;

	if(priv.jmp_point != null && get_slot_dist(priv.readhead, priv.jmp_point) < index){
		index -= get_slot_dist(priv.readhead, priv.jmp_point);
		index_start = priv.start;

		if(priv.writehead > priv.readhead){
			index -= priv.items - get_slot_dist(priv.readhead, priv.writehead);
			index_start = priv.jmp_point;
		}
	}
	else if(get_slot_dist(priv.readhead, priv.end) < index){
		index -= get_slot_dist(priv.readhead, priv.end);
		index_start = priv.start;
	}

	if(write){
		memcpy(&(((u8*)index_start)[index * slot_size + sizeof(slot_status)]), data, priv.type.size);
	}else{
		memcpy(data, &(((u8*)index_start)[index * slot_size + sizeof(slot_status)]), priv.type.size);
	}

return OK;
}

errvt methodimpl(std_Queue, Limit, u64 limit){

	nonull(self, return err);

	if(limit > priv.items){
		void* new_buf = calloc(limit, slot_size);
		std.Queue.Dequeue(self, new_buf, limit);
		priv.end = new_buf + (limit * slot_size);
		priv.readhead = new_buf;
		priv.writehead = priv.end;
	}

	priv.limit = limit;
return OK;
}
void* methodimpl(std_Queue, ToPointer){
	nonull(self, return null);

	if(priv.to_pointer_buf != null)
		free(priv.to_pointer_buf);

	priv.to_pointer_buf = calloc(priv.items, slot_size);

	slot_status* readhead = priv.readhead;
	loop(i, priv.items){

		if(readhead == priv.jmp_point)
			readhead = priv.start;

		if(*readhead == JUMP){
			pntr_shift(readhead, sizeof(slot_status));					
		
			memcpy(&((u8*)priv.to_pointer_buf)[i * priv.type.size], readhead, priv.type.size);	
		
			readhead = priv.jmp_point;					
			
		}else{
			pntr_shift(readhead, sizeof(slot_status));					
		
			memcpy(&((u8*)priv.to_pointer_buf)[i * priv.type.size], readhead, priv.type.size);	
		
			pntr_shift(readhead,priv.type.size);					
		}
	}

return priv.to_pointer_buf;
}



void* methodimpl(std_Queue, FreeToPointer){
	nonull(self, return null);

	void* result = priv.to_pointer_buf == null ? 
		std.Queue.ToPointer(self) : priv.to_pointer_buf;
	
	free(priv.start);

return result;
}

std_typeData methodimpl(std_Queue, GetType){
	nonull(self, return T(std_Nil));

return priv.type;
}

noFail methodimpl(std_Queue, Flush){
	loop(i, priv.items){
		if(priv.readhead == priv.jmp_point)
			priv.readhead = priv.start;
	
		if(*priv.readhead == JUMP){
  			*priv.readhead = EMPTY;                                                     
 			 pntr_shift(priv.readhead, slot_size);
			priv.readhead = priv.jmp_point;					
		}else{
  			*priv.readhead = EMPTY;                                                    
 			 pntr_shift(priv.readhead, slot_size);
		}
	}

	priv.items = 0;
}

ITER(std_Queue){
	nonull(self, return null)
	
	void* result = null;

	if(priv.items <= index){
		ERR(DATAERR_OUTOFRANGE, "index exceeds queue");
		return null;
	}
		
	void* index_start = priv.readhead;

	if(priv.jmp_point != null && get_slot_dist(priv.readhead, priv.jmp_point) < index){
		index -= get_slot_dist(priv.readhead, priv.jmp_point);
		index_start = priv.start;

		if(priv.writehead > priv.readhead){
			index -= priv.items - get_slot_dist(priv.readhead, priv.writehead);
			index_start = priv.jmp_point;
		}
	}
	else if(get_slot_dist(priv.readhead, priv.end) < index){
		index -= get_slot_dist(priv.readhead, priv.end);
		index_start = priv.start;
	}

	result = &(((u8*)index_start)[index * slot_size + sizeof(slot_status)]);

return result;
}

HASH(std_Queue){ return hash_bytes(std.Queue.ToPointer(self), priv.items * priv.type.size); }

SIZE(std_Queue){ return sizeof(std_Queue);}

SET(std_Queue){ 

	std.Queue.Flush(self);

	if(value != null)
	   for(int i = 0; ((void**)value)[i]; i++)
		std.Queue.Enqueue(self, ((void**)value)[i], 1);
		
return OK;
}

WRITE(std_Queue){
	loop(i, size)
	    iferr(std.Queue.Enqueue(self, data[i], 1))
		return i;
return size;
}

READ(std_Queue){
	loop(i, size)
	    iferr(std.Queue.Dequeue(self, data[i], 1))
		return i;
return size;
}

COPY(std_Queue){
	nonull(self, return nil);

	if(!create(std_Queue, 
	    where, 
	    	.type = priv.type, 
	    	.init_size = priv.items, 
	    	.literal = std.Queue.ToPointer(self)
	    )
	){ ERR(ERR_FAIL, "failed to copy queue"); return nil; } 

return where;
}

DESTROY(std_Queue){
	nonull(self, return err);

	if(priv.to_pointer_buf != null) 
		free(priv.to_pointer_buf);
	
	free(priv.start);

return OK;
}

SCAN(std_Queue){
	nonull(self, return 0);
	
	std_Queue* result = null;
	u64 len = std.DSN.Queue.parse(null, &result, in);

	if(len == 0){
		ERR(DATAERR_DSN, "failed to scan for queue");
		return 0;
	}
	*self = *result;

return len;
}

PRINT(std_Queue){
	nonull(self, return 0);

	u64 formated_len = 0; void* queue_data = null;

	if(!format || !format->debug)
		formated_len += std.DSN.Queue.format(null, self, out);

	queue_data = std.Queue.ToPointer(self);

	if(!queue_data) return 0;

	formated_len += write(out, 
		 "(Queue){\n"
		     ".len   = ",     $(priv.items),       		   ", \n",
		     ".limit = ",     $(priv.limit),			   ", \n",
		     ".type  = ",     $use(std_typeData_Type, &priv.type), ", \n",
		     ".data  = { ",
	endstr);

	loop(i, priv.items)
		formated_len += write(out, 
		  	$use((&priv.type), 
   			     pntr_shiftcpy(queue_data, i + priv.type.size)
   			), ", ",
		endstr);

	formated_len += write(out, " }\n}", endstr);

return formated_len;
}

construct(std_Queue,
FMT(),
DEF(),
){
	u64 start_size = arg.init_size  == 0 ? 10 : args->init_size;
	
	priv.start = calloc(start_size, arg.type.size + sizeof(slot_status));
	priv.type = arg.type;
	priv.items = 0;
	priv.limit = UINT64_MAX;
	priv.end = &(((u8*)priv.start)[start_size * (args->type.size + sizeof(slot_status))]);
	priv.readhead = priv.start;
	priv.writehead = priv.start;

	if(args->literal != null && args->init_size != 0){
		loop(i, start_size){
			write_queue(args->literal, i);
		}
	}

return self;
}


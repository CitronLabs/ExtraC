#include "../../../pkg.h"

import(std)
use(std_Array, List, Queue, Stack, Buffer)

errvt methodimpl(std_Array_Queue, Grow, u64 add_amount);
errvt methodimpl(std_Array_List, Grow, u64 add_amount);
errvt methodimpl(std_Array_Stack, Grow, u64 add_amount);

errvt imethodimpl(std_Array, Write, void* data, len_t len){
	self(std_Object);

	switchT(this.__type){
	caseT(List){
		List* array = generic self;

		if(len + array->items > privof(array).allocSize)
			std_Array_List_Grow(array, (privof(array).allocSize / 2) + len);

		memcpy(
		    pntr_shiftcpy(privof(array).data, array->typeSize * array->items),
		    data,
		    len * array->typeSize
		);
		
	break;}
	caseT(Queue){
		Queue* array = generic self;

		len_t allocSize = pntr_dist(privof(array).start, privof(array).end);

		if(len + array->items > allocSize)
			std_Array_Queue_Grow(array, (allocSize / 2) + len);

		loop(i, len){
			write(array, pntr_shiftcpy(data, array->typeSize * i));

		}
	break;}
	caseT(Stack){
		Stack* array = generic self;

		if(len + array->items > privof(array).allocSize)
			std_Array_Stack_Grow(array, (privof(array).allocSize / 2) + len);

		memcpy(
		    pntr_shiftcpy(privof(array).data, array->typeSize * array->items),
		    data,
		    len * array->typeSize
		);

	break;}
	caseT(Buffer){
		Buffer* array = generic self;

		len_t num_copy = len > array->items ? array->items : len;
		memcpy(
		    pntr_shiftcpy(array->data, array->typeSize * num_copy),
		    data,
		    len * array->typeSize
		);
		
	break;}
	defaultT {
		return ERR(ERR_INVALID, "invalid array type");
	}
	}

return ERR(ERR_NULLPTR, "empty type data");
}


errvt imethodimpl(std_Array, Read, void* data, len_t len){
	self(std_Object);

	switchT(this.__type){
	caseT(List){
		List* array = generic self;

		if(len + array->items > privof(array).allocSize)
			std_Array_List_Grow(array, (privof(array).allocSize / 2) + len);

		memcpy(
		    pntr_shiftcpy(privof(array).data, array->typeSize * array->items),
		    data,
		    len * array->typeSize
		);
		
	break;}
	caseT(Queue){
		Queue* array = generic self;

		len_t allocSize = pntr_dist(privof(array).start, privof(array).end);

		if(len + array->items > allocSize)
			std_Array_Queue_Grow(array, (allocSize / 2) + len);

		loop(i, len){
			write(array, pntr_shiftcpy(data, array->typeSize * i));

		}
	break;}
	caseT(Stack){
		Stack* array = generic self;

		if(len + array->items > privof(array).allocSize)
			std_Array_Stack_Grow(array, (privof(array).allocSize / 2) + len);

		memcpy(
		    pntr_shiftcpy(privof(array).data, array->typeSize * array->items),
		    data,
		    len * array->typeSize
		);

	break;}
	caseT(Buffer){
		Buffer* array = generic self;

		len_t num_copy = len > array->items ? array->items : len;
		memcpy(
		    pntr_shiftcpy(array->data, array->typeSize * num_copy),
		    data,
		    len * array->typeSize
		);
		
	break;}
	defaultT {
		return ERR(ERR_INVALID, "invalid array type");
	}
	}

return ERR(ERR_NULLPTR, "empty type data");
}

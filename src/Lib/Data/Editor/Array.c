#define IMPL_ARRAY
#include "Editor.h"

thread_local struct {
	Stack buffers;
	std_List* current;
} Process;


ArrayProc moduleFn(push)(pntr){


}
ArrayProc moduleFn(pop)(pntr){


}
ArrayProc moduleFn(insert)(pntr, len_t){


}
ArrayProc moduleFn(remove)(len_t){


}
ArrayProc moduleFn(reverse)(){


}
ArrayProc moduleFn(sort)(bool fn()(pntr a, pntr b, pntr info), pntr info){


}
ArrayProc moduleFn(each)(pntr fn()(pntr val, pntr info), pntr info){


}
ArrayProc moduleFn(filter)(bool fn()(pntr val, pntr info), pntr info){


}

std_Array_Buffer* moduleFn(makeBuffer)(){


}
std_Array_Stack* moduleFn(makeStack)(){


}
std_Array_Queue* moduleFn(makeQueue)(){


}
std_Array_List* moduleFn(makeList)(){


}

pntr moduleFn(grab)(len_t index){


}
pntr moduleFn(search)(len_t fn(search)(pntr, len_t, pntr), pntr  val){


}
pntr moduleFn(last)(){


}
pntr moduleFn(first)(){


}
pntr moduleFn(middle)(bool roundUp){


}



#pragma once
#include "../../../pkg.h"

import(std)

from(std_Array, 
     use(List),
     use(Queue),
     use(Stack),
     use(Buffer)
)


errvt std_Array_Queue_Grow(Queue* self, u64 add_amount);
errvt std_Array_Stack_Grow(Stack* self, u64 add_amount);
errvt std_Array_List_Grow(List* self, u64 add_amount);

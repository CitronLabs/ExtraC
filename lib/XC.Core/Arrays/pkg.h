#pragma once
#define __XC_CORE__
#include "../pkg.h"

#include "../Type/pkg.h"
#include "../Memory/pkg.h"

#define package std

Data(Array,
INIT(Type_t arrayType; len_t typesize, initSize; void* data; intf(std_Allocator)* allocator),
FMT(),
	len_t typesize; len_t items;
)

Interface(Array,
	values(Grow, bool,
		ATLEAST,
		EXACT
	)
	errvt  imethod(Write,    void* data, len_t num);
	errvt  imethod(Read,     void* data, len_t num);
	void*  imethod(ToPointer);
	errvt  imethod(Reserve,  bool exact, u64 amount);
	noFail imethod(Clear);
)

#undef package
#define package std_Array

#define ArrayList(type) std_Array_List*

#define createArrayList(type, ptr, size) create(std_Array_List, ptr, sizeof(type), size)
#define pushArrayList(type, size) push(std_Array_List, sizeof(type), size)
#define newArrayList(type, size) new(std_Array_List, sizeof(type), size)

Data(List, 
INIT(len_t typeSize; len_t initSize; void* data; intf(std_Allocator)* allocator),
FMT(),
	len_t typeSize, items;
private(
     	void* data;
	len_t allocSize;
    	intf(std_Allocator)* allocator;
)
);

#define ArrayQueue(type) std_Array_Queue*

#define createArrayQueue(type, ptr, size) create(std_Array_Queue, ptr, sizeof(type), size)
#define pushArrayQueue(type, size) push(std_Array_Queue, sizeof(type), size)
#define newArrayQueue(type, size) new(std_Array_Queue, sizeof(type), size)

Data(Queue,
INIT(len_t typeSize; len_t initSize; void* data; intf(std_Allocator)* allocator),
FMT(),
    len_t typeSize, items;
private(
    void
     	* writehead,
     	* readhead,
     	* start,
     	* end, 
     	* jmp_point;
    word items_til_jump;
    intf(std_Allocator)* allocator;
)
);

#define ArrayStack(type) std_Array_Stack*

#define createArrayStack(type, ptr, size) create(std_Array_Stack, ptr, sizeof(type), size)
#define pushArrayStack(type, size) push(std_Array_Stack, sizeof(type), size)
#define newArrayStack(type, size) new(std_Array_Stack, sizeof(type), size)

Data(Stack,
INIT(len_t typeSize; len_t initSize; void* data; intf(std_Allocator)* allocator),
FMT(),
	len_t typeSize, items;
private(
	void* data;
	len_t allocSize;
	intf(std_Allocator)* allocator;
)
);

#define ArrayBuffer(type) std_Array_Buffer*

#define createArrayBuffer(type, ptr, size) create(std_Array_Buffer, ptr, sizeof(type), size)
#define pushArrayBuffer(type, size) push(std_Array_Buffer, sizeof(type), size)
#define newArrayBuffer(type, size) new(std_Array_Buffer, sizeof(type), size)

#define Array(type) std_Array_Buffer
#define arr(first, ...) 			\
	((std_Array_Buffer){			\
		std_Array_Buffer_Type, 		\
		sizeof(typeof(first)),  	\
		lenof((typeof(first)[])		\
			{first, __VA_ARGS__}),  \
		(typeof(first)[])		\
  			{first, __VA_ARGS__}	\
	})

#define arr_from(buff, elem_size, len)		\
	((std_Array_Buffer){			\
		std_Array_Buffer_Type, 		\
		elem_size, len, buff  		\
	})

#define none (std_Array_Buffer){0}


Data(Buffer,
INIT(len_t typeSize; len_t initSize; void* data; intf(std_Allocator)* allocator),
FMT(),
	len_t typeSize, items; void* data;
private(
	intf(std_Allocator)* allocator;
)
);

#undef package

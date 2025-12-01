#pragma once
#define __XC_FEATURE__
#include "../pkg.h"

#include "../Type/pkg.h"

#define package std_Array

#define ArrayList(type) std_Array_List*

#define pushArrayList(type, size) push(std_Array_List, sizeof(type), size)
#define newArrayList(type, size) new(std_Array_List, sizeof(type), size)

Data(List, 
INIT(len_t typeSize; len_t initSize; void* data),
FMT(),
     	void* data;
	len_t 
	    currSize, 
	    typeSize,
	    allocSize;
);

#define ArrayQueue(type) std_Array_Queue*

#define pushArrayQueue(type, size) push(std_Array_Queue, sizeof(type), size)
#define newArrayQueue(type, size) new(std_Array_Queue, sizeof(type), size)

Data(Queue,
INIT(len_t typeSize; len_t initSize; void* data),
FMT(),
    void
     	* writehead,
     	* readhead,
     	* start,
     	* end, 
     	* jmp_point;
    len_t typeSize, items;
    word items_til_jump;
);

#define ArrayStack(type) std_Array_Stack*

#define pushArrayStack(type, size) push(std_Array_Stack, sizeof(type), size)
#define newArrayStack(type, size) new(std_Array_Stack, sizeof(type), size)

Data(Stack,
INIT(len_t typeSize; len_t initSize; void* data),
FMT(),
	void* data;
	len_t typeSize, allocSize, currSize;
);

#define ArrayBuffer(type) std_Array_Buffer*

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

#define none (std_Array_Buffer){0}


Data(Buffer,
INIT(len_t typeSize; len_t initSize; void* data),
FMT(),
	len_t typeSize, items;
	void* data;
);


#undef package

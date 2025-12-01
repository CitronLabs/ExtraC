#pragma once
#define __XC_FEATURE_BASETYPE__
#include "../pkg.h"

#define package std

#define ArrayList(type) std_ArrayList*

#define pushArrayList(type, size) push(std_ArrayList, sizeof(type), size)
#define newArrayList(type, size) new(std_ArrayList, sizeof(type), size)

Data(ArrayList, 
INIT(len_t typeSize; len_t initSize; void* data),
FMT(),
     	void* data;
	len_t 
	    currSize, 
	    typeSize,
	    allocSize;
);

#define ArrayQueue(type) std_ArrayQueue*

#define pushArrayQueue(type, size) push(std_ArrayQueue, sizeof(type), size)
#define newArrayQueue(type, size) new(std_ArrayQueue, sizeof(type), size)

Data(ArrayQueue,
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

#define ArrayStack(type) std_ArrayStack*

#define pushArrayStack(type, size) push(std_ArrayStack, sizeof(type), size)
#define newArrayStack(type, size) new(std_ArrayStack, sizeof(type), size)

Data(ArrayStack,
INIT(len_t typeSize; len_t initSize; void* data),
FMT(),
	void* data;
	len_t typeSize, allocSize, currSize;
);

#define ArrayBuffer(type) std_ArrayBuffer*

#define pushArrayBuffer(type, size) push(std_ArrayBuffer, sizeof(type), size)
#define newArrayBuffer(type, size) new(std_ArrayBuffer, sizeof(type), size)

#define Array(type) std_ArrayBuffer
#define arr(first, ...) 			\
	((std_ArrayBuffer){			\
		std_ArrayBuffer_Type, 		\
		sizeof(typeof(first)),  	\
		lenof((typeof(first)[])		\
			{first, __VA_ARGS__}),  \
		(typeof(first)[])		\
  			{first, __VA_ARGS__}	\
	})

#define none (std_ArrayBuffer){0}


Data(ArrayBuffer,
INIT(len_t typeSize; len_t initSize; void* data),
FMT(),
	len_t typeSize, items;
	void* data;
);


#undef package

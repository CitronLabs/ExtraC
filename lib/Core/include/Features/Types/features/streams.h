#pragma once
#define __XC_TYPE__
#include "../pkg.h"

#include "classes.h"



// if setMemoryAddr is set to a memory address then
// the resulting stream will write input to the memory address 

struct(Stream_Options,
	struct{ variableData data; size_t len; }init;
	void* setMemoryAddr;
	u32   
	   setMemorySize,
	   frameSize,
	   limit;
       	struct {
   		u16 readOnly  : 1;
		u16 writeOnly   : 1;
       } flags;
)



typedef void*(*StreamDecoder)(inst(Stream));
typedef errvt(*StreamEncoder)(inst(Stream), void*);

Class(Stream, 
INIT(Stream_Options ops), 
FIELD(err_t err),

	inst(Stream) 	vmethod(getStdOut);
	inst(Stream) 	fn(local)();

      	struct Stream_Proc{
		struct Stream_Proc 
	        fn(start)(inst(Stream)),
		fn(cont)(),
		fn(encode)(StreamEncoder),
		fn(decode)(StreamDecoder),
		fn(advance)(u64),
		fn(write)(void*,u64),
		fn(read)(void*,u64);

	    data(Stream)    
		* fn(pause)(),
		* fn(result)();

	    noFail fn(end)(), fn(run)();
	
	    bool 
		fn(wait)(u32);
	}Process;

);


#define __stream_for_each(type) 	\
    for (type* var = NULL;		\
	 (var = Stream.local()->__ops->Iter(Stream.local(), i)); i++) 


#define then else Stream.Process.cont()


#define decode(decoder, type) \
		decode(decoder).pause(); loop(i, 2) if(i == 0) __stream_for_each(type)

#define wait(len) 					\
		pause(); 				\
		for(bool __cont_wait; __cont_wait;) 	\
		if((__cont_wait = 			\
			Stream.Process.wait(len))	\
		)

#define run(times)   pause(); loop(i, times + 1) if(i < times)







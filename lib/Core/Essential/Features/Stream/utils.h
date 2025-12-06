#pragma once

#define __stream_for_each(type) 	\
    for (type* __var = nil;		\
	 (__var = std_Stream_Type->ops.Iter(std.Stream.Process.current(), i)); i++) 

#define each(type) 				\
	each(sizeof(type)).pause(); 		\
	loop(__forward_process, 2) 		\
	for(					\
	    const struct Stream_Proc* process = \
     		&std.Stream.Process;		\
	    __forward_process < 2;		\
	    __forward_process++)		\
		if(__forward_process == 0)	\
		    std.Stream.Process.setProc	\
			(&process);		\
		else				\
		loop(i, 2) 			\
		    if(i == 0) 			\
		__stream_for_each(type)

#define then else std.Stream.Process.cont()

#define next() next(generic &__var)

#define doDecode(decoder, _var) 		\
	doDecode(decoder).pause(); 		\
	loop(__forward_process, 2) 		\
	for(					\
	    const struct Stream_Proc* process = \
     		&std.Stream.Process;		\
	    __forward_process < 1;		\
	    __forward_process++) 		\
		if(__forward_process == 0)	\
		    std.Stream.Process.setProc	\
			(&process);		\
		else				\
		loop(i, 2) 			\
		    if(i == 0) 			\
    for (typeof(_var)* __var = &_var; decoder(process->current(), __var) == OK; i++) 


#define doRun(times)   				\
	pause(); 				\
	loop(__forward_process, 2) 		\
	for(					\
	    const struct Stream_Proc* process = \
     		&std.Stream.Process;		\
	    __forward_process < 1;		\
	    __forward_process++) 		\
		if(__forward_process == 0)	\
		    std.Stream.Process.setProc	\
			(&process);		\
		else				\
     		loop(i, (times)) 		\
		    if(i < (times))

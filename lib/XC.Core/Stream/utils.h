#pragma once

#define each(type) 					\
	each(); 					\
	foreach(std.Stream.Process.result(), type, val)

#define next() next(generic &__var)

#define doDecode(decoder, _var) 		\
	doDecode(decoder); 			\
    for (typeof(_var)* __var = &_var; 		\
	 decoder(std.Stream.Process.result(), 	\
	 __var) == OK;) 



#pragma once
#define __XC_TYPES__
#include "../pkg.h"
#include "../types.h"

#define WRITE(class)	 u64   methodimpl(class, Op_Write, pntr data[], len_t size)			
#define READ(class)	 u64   methodimpl(class, Op_Read , pntr data[], len_t size)			
#define SET(class)     	 errvt methodimpl(class, Op_Set  , void* value)			
#define GET(class)       errvt methodimpl(class, Op_Get  , void* buff);
#define COPY(class)      void* methodimpl(class, Op_Copy , void* where)			
#define HASH(class)      u32   methodimpl(class, Op_Hash)				
#define ITER(class)      void* methodimpl(class, Op_Iter , u64 index)				
#define SCAN(class)   	 u64   methodimpl(class, Op_Scan , class##_FormatArgs* format,  std_Stream* in)	
#define PRINT(class)  	 u64   methodimpl(class, Op_Print, class##_FormatArgs* format,  std_Stream* out)	
#define SIZE(class)   	 len_t methodimpl(class, Op_Size , bool elements)
 

Interface(Data_Ops,
	u64   fn(writeTo) (std_varData type, pntr data[], len_t size);
	u64   fn(readFrom)(std_varData type, pntr data[], len_t size);
	errvt fn(setTo)   (std_varData type, pntr value);
	errvt fn(getFrom) (std_varData type, pntr buff);
	void* fn(copyTo)  (std_varData type, pntr where);
	len_t fn(sizeOf)  (std_varData type, bool elements);
	void* fn(indexOf) (std_varData type, u64 index);
	u32   fn(hashOf)  (std_varData type);
)


#define write(var, ...)  	  std.Type.data.writeTo(V(asObject(var)), (void*[]){__VA_ARGS__}, __VA_ARG_LEN__(type, __VA_ARGS__))
#define write_use(type, var, ...)  std.Type.data.writeTo((std_varData){generic type, .data = generic var}, (void*[]){__VA_ARGS__}, __VA_ARG_LEN__(type, __VA_ARGS__))

#define read(var, ...)  	  std.Type.data.readFrom(V(asObject(var)), (void*[]){__VA_ARGS__}, __VA_ARG_LEN__(type, __VA_ARGS__))
#define read_use(type, var, ...)   std.Type.data.readFrom((std_varData){generic type, .data = generic var}, (void*[]){__VA_ARGS__}, __VA_ARG_LEN__(type, __VA_ARGS__))

#define set(var, to)      std.Type.data.setTo(V(asObject(var)), to)
#define copy(var, to)     std.Type.data.copyTo(V(asObject(var)), to)
#define hash(var)         std.Type.data.hashOf(V(asObject(var)))
#define index(var, index) std.Type.data.indexOf(V(asObject(var)), index)

#define set_use(type, var, to)      std.Type.data.setTo  ((std_varData){generic type, .data = generic var}, to)
#define copy_use(type, var, to)     std.Type.data.copyTo ((std_varData){generic type, .data = generic var}, to)
#define hash_use(type, var)         std.Type.data.hashOf ((std_varData){generic type, .data = generic var})
#define index_use(type, var, index) std.Type.data.indexOf((std_varData){generic type, .data = generic var}, index)

#define elements(var)     std.Type.data.sizeOf(V(asObject(var)), true)
#define size(var)         std.Type.data.sizeOf(V(asObject(var)), false)

#define elements_use(type, var)     std.Type.data.sizeOf((std_varData){generic type, .data = generic var}, true)
#define size_use(type, var)         std.Type.data.sizeOf((std_varData){generic type, .data = generic var}, false)



#define get(var, to)      static_assert(false, "currently unimplemented reserving for future use");
			  //std.Type.data.getFrom(V(asObject(var)), to)

#define get_use(type, var, to)    static_assert(false, "currently unimplemented reserving for future use"); 
				  //std.Type.data.getFrom((std_varData){generic &type, .data = generic var}, to)

u32 hash_bytes(void* data_ptr, u64 size);



/**
@def foreach(iterable, type, var)
@param iterable any object who's class has implemented the ITER interface
@param type the type of a single member within the iterable object
@param var the name of the varible which stores the current member of the iteration
*/
#define foreach(iterableVar, _type, _var) 					\
	std_typeOperators* _var##_ops = generic &(iterableVar)->__type->ops;	\
	void*          _var##_iterable = (iterableVar);				\
	_type* _var = null;							\
	for(len_t _var##_iterator = 0; 						\
     	    (_var = _var##_ops->						\
		Iter(_var##_iterable, _var##_iterator)); 			\
	    	_var##_iterator++)						\
	


#define HASHMAP_HASH_INIT 2166136261u



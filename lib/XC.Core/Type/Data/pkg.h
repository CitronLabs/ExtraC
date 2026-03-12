#pragma once
#define __XC_TYPES__
#include "../pkg.h"
#include "../types.h"

#define package std

#define WRITE(class)	 u64   moduleMethod(class, Op_Write, pntr data[], len_t size)			
#define READ(class)	 u64   moduleMethod(class, Op_Read , pntr data[], len_t size)			
#define SET(class)     	 errvt moduleMethod(class, Op_Set  , void* value)			
#define GET(class)       errvt moduleMethod(class, Op_Get  , void* buff);
#define COPY(class)      void* moduleMethod(class, Op_Copy , void* where)			
#define HASH(class)      u32   moduleMethod(class, Op_Hash)				
#define ITER(class)      void* moduleMethod(class, Op_Iter , u64 index)				
#define SCAN(class)   	 u64   moduleMethod(class, Op_Scan , class##_FormatArgs* format,  std_Stream* in)	
#define PRINT(class)  	 u64   moduleMethod(class, Op_Print, class##_FormatArgs* format,  std_Stream* out)	
#define SIZE(class)   	 len_t moduleMethod(class, Op_Size , bool elements)
 

Interface(Data_Ops,
	len_t fn(readFrom)(std_varData type, pntr data[], len_t size);
	len_t fn(writeTo) (std_varData type, constpntr data[], len_t size);
	errvt fn(setTo)   (std_varData type, constpntr value);
	errvt fn(getFrom) (std_varData type, constpntr buff);
	void* fn(copyTo)  (std_varData type, constpntr where);
	len_t fn(sizeOf)  (std_varData type, bool elements);
	void* fn(indexOf) (std_varData type, len_t index);
	u32   fn(hashOf)  (std_varData type);
)


#define write(var, ...)  	  std.Types.data.writeTo(V(asObject(var)), (constpntr[]){__VA_ARGS__}, __VA_ARG_LEN__(type, __VA_ARGS__))
#define write_use(type, var, ...) std.Types.data.writeTo((std_varData){generic type, .data = generic var}, (constpntr[]){__VA_ARGS__}, __VA_ARG_LEN__(type, __VA_ARGS__))

#define read(var, ...)  	  std.Types.data.readFrom(V(asObject(var)), (void*[]){__VA_ARGS__}, __VA_ARG_LEN__(type, __VA_ARGS__))
#define read_use(type, var, ...)   std.Types.data.readFrom((std_varData){generic type, .data = generic var}, (void*[]){__VA_ARGS__}, __VA_ARG_LEN__(type, __VA_ARGS__))

#define set(var, to)      std.Types.data.setTo(V(asObject(var)), to)
#define copy(var, to)     std.Types.data.copyTo(V(asObject(var)), to)
#define hash(var)         std.Types.data.hashOf(V(asObject(var)))
#define index(var, index) std.Types.data.indexOf(V(asObject(var)), index)

#define set_use(type, var, to)      std.Types.data.setTo  ((std_varData){generic type, .data = generic var}, to)
#define copy_use(type, var, to)     std.Types.data.copyTo ((std_varData){generic type, .data = generic var}, to)
#define hash_use(type, var)         std.Types.data.hashOf ((std_varData){generic type, .data = generic var})
#define index_use(type, var, index) std.Types.data.indexOf((std_varData){generic type, .data = generic var}, index)

#define len(var)     	  std.Types.data.sizeOf(V(asObject(var)), true)
#define size(var)         std.Types.data.sizeOf(V(asObject(var)), false)

#define len_use(type, var)     	    std.Types.data.sizeOf((std_varData){generic type, .data = generic var}, true)
#define size_use(type, var)         std.Types.data.sizeOf((std_varData){generic type, .data = generic var}, false)



#define get(var, to)      static_assert(false, "currently unimplemented reserving for future use");
			  //std.Types.data.getFrom(V(asObject(var)), to)

#define get_use(type, var, to)    static_assert(false, "currently unimplemented reserving for future use"); 
				  //std.Types.data.getFrom((std_varData){generic &type, .data = generic var}, to)

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
	_type* _var = nil;							\
	for(len_t _var##_iterator = 0; 						\
     	    (_var = _var##_ops->						\
		Iter(_var##_iterable, _var##_iterator)) == nil;			\
	    	_var##_iterator++)						\
	


#define HASHMAP_HASH_INIT 2166136261u

#undef package

#pragma once
#include "basetypes.h"
#include "classes.h"
#include "err.h"

#define initialize(name, ptr, ...) name##_Construct((name##_ConstructArgs){__VA_ARGS__}, ptr)

#define new(name, ...) initialize(name, malloc(name##__Size(NULL, false)), __VA_ARGS__)
#define push(name, ...) initialize(name, alloca(name##__Size(NULL, false)), __VA_ARGS__)

#define del(...) __XC_Object_Destroy(true, (pntr[]){__VA_ARGS__}, sizeof((pntr[]){__VA_ARGS__}) / sizeof(pntr))
#define pop(...) __XC_Object_Destroy(false, (pntr[]){__VA_ARGS__}, sizeof((pntr[]){__VA_ARGS__}) / sizeof(pntr))


#define __APPEND(class)	 errvt imethodimpl(Class, _HASH,, u32* result)				
#define __SET(class)     errvt imethodimpl(Class, _DESTROY)					
#define __COPY(class)    u64   imethodimpl(Class, _APPEND,, pntr data[])			
#define __SIZE(class)    errvt imethodimpl(Class, _SET,,    void* value)			
#define __HASH(class)    errvt imethodimpl(Class, _COPY,,   void* where)			
#define __DESTROY(class) errvt imethodimpl(Class, _SIZE,,   size_t* result, bool elements)		
#define __ITER(class)    void* imethodimpl(Class, _ITER,,   u64 index)				

                        
#define append(var, ...)  (var)->__ops->Append(var, (void*[]){__VA_ARGS__})
#define set(var, to)      (var)->__ops->Set(var, to)
#define copy(var, to)     (var)->__ops->Copy(var, to)
#define size(var)         (var)->__ops->Size(var, false)
#define elements(var)     (var)->__ops->Size(var, true)
#define hash(var)         (var)->__ops->Hash(var)

#define setAllOps(Class) 			\
		.Append  = Class##__Append, 	\
		.Set     = Class##__Set,	\
		.Copy    = Class##__Copy,	\
		.Size    = Class##__Size,	\
		.Hash    = Class##__Hash,	\
		.Destroy = Class##__Destroy,	\
		.Iter    = Class##__Iter,	\
		.Scan    = Class##__Scan,	\
		.Print   = Class##__Print,	\
		

#define hash_bytes(pntr, size) __XC_Default_Hash(pntr, size)	

static inline errvt __XC_Object_Destroy(bool doFree, void* delObjs[], size_t delObjsNum){

	for(size_t i = 0; i < delObjsNum; i++) {
	    if(((typeData*) delObjs[i])->ops->Destroy == NULL) 
		return ERR(ERR_NULLPTR,"no destructor specified for this object"); 

	    else iferr(((typeData*) delObjs[i])->ops->Destroy(generic delObjs[i]))
		{return err;}


	    if (doFree) free(delObjs[i]);
	}

return OK;
}

#define HASHMAP_HASH_INIT 2166136261u
static inline u32 __XC_Default_Hash(void* data_ptr, u64 size){

	char* data = data_ptr;
	size_t nblocks = size / 8;
	u64 hash = HASHMAP_HASH_INIT;
	for (size_t i = 0; i < nblocks; ++i)
	{
		hash ^=  (u64)data[0] << 0 | (u64)data[1] << 8 |
			 (u64)data[2] << 16 | (u64)data[3] << 24 |
			 (u64)data[4] << 32 | (u64)data[5] << 40 |
			 (u64)data[6] << 48 | (u64)data[7] << 56;
		hash *= 0xbf58476d1ce4e5b9;
		data += 8;
	}

	u64 last = size & 0xff;
	switch (size % 8)
	{
	case 7:
		last |= (u64)data[6] << 56; 
	case 6:
		last |= (u64)data[5] << 48; 
	case 5:
		last |= (u64)data[4] << 40; 
	case 4:
		last |= (u64)data[3] << 32; 
	case 3:
		last |= (u64)data[2] << 24; 
	case 2:
		last |= (u64)data[1] << 16; 
	case 1:
		last |= (u64)data[0] << 8;
		hash ^= last;
		hash *= 0xd6e8feb86659fd93;
	}

return (u32)(hash ^ hash >> 32);
}

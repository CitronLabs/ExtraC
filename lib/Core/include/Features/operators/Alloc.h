#pragma once
#define __XC_FEATURE__
#include "../pkg.h"
#include "../Types/pkg.h"


#define initialize(name, ptr, freeOnFail, ...) 	\
	__XC_Object_Create(			\
		(typeData){			\
		  (void*)&name##_Ops, 		\
		  name##__Size(NULL, false, 	\
		  	&(name##_ConstructArgs)	\
		      	{__VA_ARGS__}		\
		  )				\
		},  		    		\
		name##_hasPrivate ? 		\
			sizeof(data(name)) : 0, \
		freeOnFail, 			\
		ptr, 				\
		&(name##_ConstructArgs)		\
   			{__VA_ARGS__}		\
	)

#define new(name, ...) initialize(name, malloc(name##__Size(NULL, false, &(name##_ConstructArgs){__VA_ARGS__})), true, __VA_ARGS__)
#define push(name, ...) initialize(name, alloca(name##__Size(NULL, false, &(name##_ConstructArgs){__VA_ARGS__})), false, __VA_ARGS__)

#define del(...) __XC_Object_Destroy(true, (pntr[]){__VA_ARGS__}, sizeof((pntr[]){__VA_ARGS__}) / sizeof(pntr))
#define pop(...) __XC_Object_Destroy(false, (pntr[]){__VA_ARGS__}, sizeof((pntr[]){__VA_ARGS__}) / sizeof(pntr))

#define __SIZE(class)    size_t imethodimpl(Class, _SIZE,,   bool elements, name##_ConstructArgs* args)
#define __DESTROY(class) errvt imethodimpl(Class, _DESTROY,, bool* doFree)							
#define __CREATE(class)  name##_Instance* name##_CREATE(name##_Instance* self, size_t size, name##_ConstructArgs* args)


#define size(var)         (var)->__ops->Size(var, false)


void* __XC_Object_Create(typeData type, size_t privateOffset, bool freeOnFail, inst(Object) self, void* args){ 

	if(privateOffset)
		self->__private = pntr_shiftcpy(self, privateOffset);						

	self->__ops = type.ops;

	if(type.ops->Create(self, type.size ,args) == NULL){ 		
	    ERR(ERR_INITFAIL, "failed to initialize object");	
	    return self;}					
return self;
}								

#define construct(name, ...) Impl(name){__VA_ARGS__}; __CREATE(name)


static inline errvt __XC_Object_Destroy(bool doFree, void* delObjs[], size_t delObjsNum){

	for(size_t i = 0; i < delObjsNum; i++) {
	    if(((typeData*) delObjs[i])->ops->Destroy == NULL) 
		return ERR(ERR_NULLPTR,"no destructor specified for this object"); 

	    else iferr(((typeData*) delObjs[i])->ops->Destroy(generic delObjs[i], &doFree))
		{return err;}


	    if (doFree) free(delObjs[i]);
	}

return OK;
}






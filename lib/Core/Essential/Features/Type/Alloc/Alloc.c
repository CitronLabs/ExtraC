#include "../../../../pkg.h"

import(std);
;

use(std, typeData, Object);

void* __XC_Object_Create(typeData* type, bool freeOnFail, Object* self, void* args){ 

	self->__type = generic type;

	len_t obj_size = size_use(type, self);
	void* restore_buff = alloca(obj_size);

	memcpy(restore_buff, self, obj_size);

	if(isImpl(ops(type).Create(self, type->size ,args))){ 		
	    ERR(ERR_INITFAIL, "failed to initialize object");	

	    if(freeOnFail) 
		free(self);
	    else
		memcpy(self, restore_buff, obj_size);

	    return nil;
	}					
return self;
}								



errvt __XC_Object_Destroy(bool doFree, void* delObjs[], len_t delObjsNum){

	for(len_t i = 0; i < delObjsNum; i++) {
	    if(isImpl(ops((typeData*) delObjs[i]).Destroy)) 
		iferr(ops((typeData*) delObjs[i]).Destroy(generic delObjs[i]))
		    {return err;}


	    if (doFree) free(delObjs[i]);
	}

return OK;
}

#include "../../../../pkg.h"

import(std)


PRINT(std_typeData){

	return write(out, 
	    "(typeData){ ",
	        ".ops  = ", $use(std_typeOperators_Type, &self->ops), ", ",
	        ".size = ", $(self->size), ", ",
	        ".id   = ", $((void*)self->id),
	    " }",
	fmt_end);

}

HASH(std_typeData){
return pntr_asVal(self->id);
}

COPY(std_typeData){
	memcpy(where, self, sizeof(std_typeData));
return where;
}

SIZE(std_typeData){
	return elements ? self->size : sizeof(std_typeData);
}

DESTROY(std_typeData){
	if(self) free(self);
return OK;
}

SET(std_typeData){
	*self = *(std_typeData*)value;
return OK;
}

construct(std_typeData,
FMT(),
DEF(),
	.Print   = std_typeData_Op_Print,
	.Hash    = std_typeData_Op_Hash,
	.Size    = std_typeData_Op_Size,
	.Copy    = std_typeData_Op_Copy,
	.Create  = std_typeData_Op_Create,
	.Destroy = std_typeData_Op_Destroy,
	.Set	 = std_typeData_Op_Set,
	.Scan    = nilmethod,
	.Iter 	 = nilmethod,
	.Write   = nilmethod,
	.Read 	 = nilmethod,

){ passover }


PRINT(std_typeOperators){

	return write(out, 
	    "(typeOperators){ ",
		".Print =",   self->Print   == nilmethod ? "nil" : "implemented", ", ",
		".Scan =",    self->Scan    == nilmethod ? "nil" : "implemented", ", ",
		".Create =",  self->Create  == nilmethod ? "nil" : "implemented", ", ",
		".Destroy =", self->Destroy == nilmethod ? "nil" : "implemented", ", ",
		".Write =",   self->Write   == nilmethod ? "nil" : "implemented", ", ",
		".Read =",    self->Read    == nilmethod ? "nil" : "implemented", ", ",
		".Iter =",    self->Iter    == nilmethod ? "nil" : "implemented", ", ",
		".Set =",     self->Set     == nilmethod ? "nil" : "implemented", ", ",
		".Hash =",    self->Hash    == nilmethod ? "nil" : "implemented", ", ",
		".Size =",    self->Size    == nilmethod ? "nil" : "implemented", ", ",
		".Copy =",    self->Copy    == nilmethod ? "nil" : "implemented", ", ",

	    " }",
	fmt_end);

}

COPY(std_typeOperators){
	memcpy(where, self, sizeof(std_typeOperators));
return where;
}

SIZE(std_typeOperators){
	return sizeof(std_typeOperators);
}

DESTROY(std_typeOperators){
	if(self) free(self);
return OK;
}

SET(std_typeOperators){
	*self = *(std_typeOperators*)value;
return OK;
}

construct(std_typeOperators,
FMT(),
DEF(),
	.Print   = std_typeOperators_Op_Print,
	.Size    = std_typeOperators_Op_Size,
	.Copy    = std_typeOperators_Op_Copy,
	.Create  = std_typeOperators_Op_Create,
	.Destroy = std_typeOperators_Op_Destroy,
	.Set	 = std_typeOperators_Op_Set,
	.Hash    = nilmethod,
	.Scan    = nilmethod,
	.Iter 	 = nilmethod,
	.Write   = nilmethod,
	.Read 	 = nilmethod,
){ passover }


PRINT(std_varData){
	return write(out, 
	    "(varData){ ",
	    	".type = ", $use(std_typeData_Type, &self->type), ", ",
	    	".data = ", $use(&self->type, self->data), ", ",
	    " }",
	fmt_end);
}

COPY(std_varData){
	memcpy(where, self, sizeof(std_varData));
return where;
}

HASH(std_varData){
	return ops(&self->type).Hash(self->data);

}

SIZE(std_varData){
	return elements ? self->type.size : sizeof(std_varData);
}

DESTROY(std_varData){
	if(self) free(self);
return OK;
}

SET(std_varData){
	*self = *(std_varData*)value;
return OK;
}

construct(std_varData,
FMT(),
DEF(),
	.Print   = std_varData_Op_Print,
	.Size    = std_varData_Op_Size,
	.Copy    = std_varData_Op_Copy,
	.Create  = std_varData_Op_Create,
	.Destroy = std_varData_Op_Destroy,
	.Set	 = std_varData_Op_Set,
	.Hash    = std_varData_Op_Hash,
	.Scan    = nilmethod,
	.Iter 	 = nilmethod,
	.Write   = nilmethod,
	.Read 	 = nilmethod,
){ passover }


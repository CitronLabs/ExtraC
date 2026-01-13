#include <XC.Core/pkg.c>

#define module std, typeData

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
	.Scan    = nil,
	.Iter 	 = nil,
	.Write   = nil,
	.Read 	 = nil,

){ passover }

#undef module
#define module std, typeOperators


PRINT(std_typeOperators){

	return write(out, 
	    "(typeOperators){ ",
		".Print =",   self->Print   == nil ? "nil" : "implemented", ", ",
		".Scan =",    self->Scan    == nil ? "nil" : "implemented", ", ",
		".Create =",  self->Create  == nil ? "nil" : "implemented", ", ",
		".Destroy =", self->Destroy == nil ? "nil" : "implemented", ", ",
		".Write =",   self->Write   == nil ? "nil" : "implemented", ", ",
		".Read =",    self->Read    == nil ? "nil" : "implemented", ", ",
		".Iter =",    self->Iter    == nil ? "nil" : "implemented", ", ",
		".Set =",     self->Set     == nil ? "nil" : "implemented", ", ",
		".Hash =",    self->Hash    == nil ? "nil" : "implemented", ", ",
		".Size =",    self->Size    == nil ? "nil" : "implemented", ", ",
		".Copy =",    self->Copy    == nil ? "nil" : "implemented", ", ",

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
	.Hash    = nil,
	.Scan    = nil,
	.Iter 	 = nil,
	.Write   = nil,
	.Read 	 = nil,
){ passover }

#undef module
#define module std, varData

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
	.Scan    = nil,
	.Iter 	 = nil,
	.Write   = nil,
	.Read 	 = nil,
){ passover }


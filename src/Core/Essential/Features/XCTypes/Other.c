#include <Core/pkg.c>

/*----------------------------------------------
 *						|
 *		Boolean Operations		|
 *						|
-----------------------------------------------*/

#define module bool

PRINT(bool){
	return printTo(out,
		self ? fmt_skip : 
	       	*self ? "true" : "false",
	fmt_end);
}

SCAN(bool){
	len_t len_bool_txt = 0, prev_pos = size(in);
	rune c = 0;
	std.Stream.Process
	    .start(in)
	    .doDecode(std.String.UTF8.Decoder, c){
		
		while(iswblank(c)) process->next();

		if(
		    !(len_bool_txt = scanFrom(in, "true"))  ||
		    !(len_bool_txt = scanFrom(in, "false")) 
		){
			ERR(ERR.INVALID, "invalid boolean, must be either true or false");
			process->fail();
			return 0;
		}
		
		if(len_bool_txt == sizeof("true"))
			*self = true;
		else
		*self = false;


	    }
	    then.end()
	;

return prev_pos - size(in);
}

HASH(bool){
	return hash_bytes(
		*self ? 
			&(u8){1} : &(u8){0},
		sizeof(u8)
	);
}

SET(bool){
	*self = *(bool*)value;
return OK;
}

SIZE(bool){
	return sizeof(bool);
}

COPY(bool){
	memcpy(where, self, sizeof(bool));
return where;
}

DESTROY(bool){ return OK; }

construct(bool,
FMT(),
DEF(),
	.Scan    = bool_Op_Scan,
	.Print   = bool_Op_Print,
	.Hash    = bool_Op_Hash,
	.Size    = bool_Op_Size,
	.Copy    = bool_Op_Copy,
	.Create  = bool_Op_Create,
	.Destroy = bool_Op_Destroy,
	.Set	 = bool_Op_Set,
	.Iter 	 = nil,
	.Write   = nil,
	.Read 	 = nil,
){ passover }

/*----------------------------------------------
 *						|
 *		Pointer Operations		|
 *						|
-----------------------------------------------*/

#undef module
#define module pntr

PRINT(pntr){

return printTo(out, "0x",$(pntr_asVal(*self)));
}

SCAN(pntr){

	rune c = 0;
	void* result;

	len_t scannedLen = 0;

	if(!(scannedLen = scanFrom(in, "0x", $use(len_t_Type, &result)))){
	    ERR(ERR.INVALID, "failed to scan for pointer value");
	    return 0;
	}

	if(create(pntr, self, result) == nil){
	    ERR(ERR.FAIL, "failed to create pointer object");
	    return 0;
	}

return scannedLen;
}

HASH(pntr){
return hash_bytes(self, sizeof(void*));
}

COPY(pntr){
	memcpy(where, self, sizeof(void*));
return where;
}

SIZE(pntr){
	return sizeof(void*);
}

DESTROY(pntr){ return OK; }

SET(pntr){
	*self = *(void**)value;
return OK;
}

construct(pntr,
FMT(),
DEF(),
	.Scan    = pntr_Op_Scan,
	.Print   = pntr_Op_Print,
	.Hash    = pntr_Op_Hash,
	.Size    = pntr_Op_Size,
	.Copy    = pntr_Op_Copy,
	.Create  = pntr_Op_Create,
	.Destroy = pntr_Op_Destroy,
	.Set	 = pntr_Op_Set,
	.Iter 	 = nil,
	.Write   = nil,
	.Read 	 = nil,
){ passover }

#undef module
#define module constpntr

construct(constpntr,
FMT(),
DEF(),
	.Scan    = generic pntr_Op_Scan,
	.Print   = generic pntr_Op_Print,
	.Hash    = generic pntr_Op_Hash,
	.Size    = generic pntr_Op_Size,
	.Copy    = generic pntr_Op_Copy,
	.Create  = generic pntr_Op_Create,
	.Destroy = generic pntr_Op_Destroy,
	.Set	 = generic pntr_Op_Set,
	.Iter 	 = nil,
	.Write   = nil,
	.Read 	 = nil,
);

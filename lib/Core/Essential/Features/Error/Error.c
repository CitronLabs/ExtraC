#include "../../../pkg.h"

import(std)
import(XC)


typedef struct {
	errvt* errors_to_catch; len_t errors_to_catch_len;
	stateData try_throw_jumppoint;
	u8 trying : 1, showErrors : 1;
}errorState;

std_Local* local_errState;


#define ERR_MSG(msg) XC.Dev.Stream.writeTo(XC.Dev.Stream.stdHandle(XC.Dev.Stream.ID.Err), msg, sizeof(msg))

static inline errorState* fetchErrState(){

	errorState* err_state;

	if(!local_errState){
		local_errState = new(std_Local, sizeof(std_Error));
		
	  	if(local_errState == nil){
	  		ERR_MSG(
			    RED"[CRITICAL ERROR] ERR_INITFAIL\n"
	     		    "errormsg: Failed to initialize the local error state value\n\n"
	     		);
			XC.Sys.terminate(XC.Sys.ExitCode.FAILURE, 0);
		}
		
		err_state = std.Local.getData(local_errState);
		
		err_state->showErrors = true;
	} else {
		err_state = std.Local.getData(local_errState);
	}

return err_state;
}

std_Error* std_err_Get(){
	static std_Local* local_err = null;

	if(!local_err){
		local_err = new(std_Local, sizeof(std_Error));
		
	  	if(local_err == nil){
	  		ERR_MSG(
			    RED"[CRITICAL ERROR] ERR_INITFAIL\n"
	     		    "errormsg: Failed to initialize the local error state value\n\n"
	     		);
			XC.Sys.terminate(XC.Sys.ExitCode.FAILURE, 0);
		}
	}
		
return std.Local.getData(local_err);
}

errvt std_err_Set(std_Error* err, const strc8 err_name, const char funcname[]){

	std_Error*  local_err = std.Error.Get();
	errorState* err_state = fetchErrState();
	u8 count = 0;
	
	*local_err = *err;
	
return err->errorcode;
}



noFail std_err_Hide(){
	fetchErrState()->showErrors = false;
}


noFail std_err_Show(){
	fetchErrState()->showErrors = true;
}

noFail std_err_Clear(){
	std_Error* local_err  = std.Error.Get();
	errorState* err_state = fetchErrState();

	local_err->errorcode = 0;
	local_err->message = "No Error";
	if(err_state->trying) {
		err_state->trying = false;
		err_state->try_throw_jumppoint = null;
	}
}

errvt std_err_Try(errvt* errors_to_catch, len_t num){
	std_Error* local_err  = std.Error.Get();
	errorState* err_state = fetchErrState();

	XC.Sys.Arch.getInstructionPtr();
	if(XC.Sys.saveState(&err_state->try_throw_jumppoint)){
		err_state->trying = false;
		return local_err->errorcode;
	}
	err_state->errors_to_catch 	= errors_to_catch;
	err_state->errors_to_catch_len 	= num;

	err_state->trying = true;

return OK;
}

void std_err_Throw(){
	errorState* err_state = fetchErrState();
	
	if(err_state->trying)
	    XC.Sys.loadState(err_state->try_throw_jumppoint);
}

PRINT(std_Error){
return write(out,
	 RED"[ERROR] ", self->message, "\n");
}

SET(std_Error){
	memcpy(self, value, sizeof(std_Error));
return OK;
}

HASH(std_Error){
	return hash_bytes(
		&(u32){ 
		    hash_bytes(&self->errorcode, sizeof(errvt)) + 
		    hash_bytes(self->message, strnlen(self->message, 4080))
		},
		sizeof(u32)
	);
}

SIZE(std_Error){
	return elements ? sizeof(errvt) + strnlen(self->message, 4080) : sizeof(std_Error);
}

COPY(std_Error){
	memcpy(where, self, sizeof(std_Error));
return where;
}

DESTROY(std_Error){
	return OK;
}

CREATE(std_Error);

construct(std_Error,
FMT(),
DEF(),
	.Print 	 = std_Error_Op_Print,
	.Set 	 = std_Error_Op_Set,
	.Hash	 = std_Error_Op_Hash,
	.Copy	 = std_Error_Op_Copy,
	.Destroy = std_Error_Op_Destroy,
	.Size 	 = std_Error_Op_Size,
	.Create	 = std_Error_Op_Create,
	.Iter 	 = nilmethod,
	.Scan 	 = nilmethod,
	.Write   = nilmethod,
	.Read	 = nilmethod,
){
	self->errorcode = arg.errorcode;
	self->message   = arg.message;
	
return self;
}

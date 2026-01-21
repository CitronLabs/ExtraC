#include <XC.pkg.c>

#define module std, Error

typedef struct {
	errvt* 		errors_to_catch; 
	len_t 		errors_to_catch_len;
	std_Stream** 	error_output;
	stateData 	try_throw_jumppoint;
	const char* 	err_func,* err_module,* err_name;
	u8 		trying : 1, showErrors : 1;
}errorState;



errorState* moduleFn(fetchErrState)(){
	thread_local static errorState err_state;

return &err_state;
}

std_Error* moduleFn(Get)(){
	thread_local static std_Error local_err = {0};

return &local_err;
}

errvt moduleFn(Set)(std_Error* err, const strc8 err_name, const char funcname[], const char modulename[]){

	std_Error*  local_err = std.Error.Get();
	errorState* err_state = mod(fetchErrState)();
	u8 count = 0;
	
	*local_err = *err;

	err_state->err_func   = funcname;
	err_state->err_module = modulename;
	err_state->err_name   = err_name;

	if(err_state->showErrors && err_state->error_output != nil){
		printTo(*err_state->error_output, $(local_err));
	}
	
return err->errorcode;
}



noFail moduleFn(Hide)(){
	mod(fetchErrState)()->showErrors = false;
}


noFail moduleFn(Show)(){
	mod(fetchErrState)()->showErrors = true;
}

noFail moduleFn(Clear)(){
	std_Error* local_err  = std.Error.Get();
	errorState* err_state = mod(fetchErrState)();

	local_err->errorcode = 0;
	local_err->message = "No Error";
	if(err_state->trying) {
		err_state->trying = false;
		err_state->try_throw_jumppoint = nil;
	}
}

errvt moduleFn(Try)(errvt* errors_to_catch, len_t num){
	std_Error* local_err  = std.Error.Get();
	errorState* err_state = mod(fetchErrState)();

	if(core.System.saveState(&err_state->try_throw_jumppoint)){
		err_state->trying = false;
		return local_err->errorcode;
	}
	err_state->errors_to_catch 	= errors_to_catch;
	err_state->errors_to_catch_len 	= num;

	err_state->trying = true;

return OK;
}

errvt moduleFn(Throw)(){
	std_Error*  local_err = std.Error.Get();
	errorState* err_state = mod(fetchErrState)();
	
	if(err_state->trying){
	    if(err_state->errors_to_catch_len == 0)
		core.System.loadState(err_state->try_throw_jumppoint);

	    else loop(i, err_state->errors_to_catch_len)
		if(local_err->errorcode == err_state->errors_to_catch[i])
	    	    core.System.loadState(err_state->try_throw_jumppoint);
	}

return local_err->errorcode;
}

PRINT(std_Error){
	errorState* err_state = mod(fetchErrState)();

return printTo(out,
	 RED,"[ERROR] ",NC, err_state->err_name,
	       " in ",      err_state->err_module,
	       " at ",      err_state->err_func,
	       ": ",        this.message
	);
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
	.Print 	 = mod(Op_Print),
	.Set 	 = mod(Op_Set),
	.Hash	 = mod(Op_Hash),
	.Copy	 = mod(Op_Copy),
	.Destroy = mod(Op_Destroy),
	.Size 	 = mod(Op_Size),
	.Create	 = mod(Op_Create),
	.Iter 	 = nil,
	.Scan 	 = nil,
	.Write   = nil,
	.Read	 = nil,
){ passover }

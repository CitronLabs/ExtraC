#include "../../pkg.h"

import(std)




std_Logger* stdLogger = null;

std_Logger*  Logger_getStdLogger(){ return stdLogger; }
errvt 	     Logger_setStdLogger(std_Logger* logger){nonull(logger, return err); stdLogger = logger; return OK;}


errvt methodimpl(std_Logger, log, std_logID log, strc8 message){
	nonull(self, return err);

	if(log > priv.logs.currSize)
		return ERR(ERR_INVALID, "invalid logID");

	if(printTo((std_Stream*)index(&priv.logs, log), message) == 0)
		return ERR(ERR_FAIL, "failed to format text for log");

return OK;
}

errvt methodimpl(std_Logger, logTo, strc8 name, strc8 message){
	nonull(self, return err);
	nonull(name, return err);
	nonull(this.nameLookup, return err);
	
	std_logID log = self->nameLookup(name);

	if(log > priv.logs.currSize)
		return ERR(ERR_INVALID, "invalid logID");

	if(printTo((std_Stream*)index(&priv.logs, log), message) == 0)
		return ERR(ERR_FAIL, "failed to format text for log");
return OK;
}

std_logID methodimpl(std_Logger, newLog, std_Stream* stream){
	nonull(self,   return err);
	nonull(stream, return err);

	std_logID id = LOGGER_null; 

	if(!(id = write(&priv.logs, stream))){
		ERR(ERR_FAIL, "failed to add new log");
		return LOGGER_null;
	}

return id;
}

std_logID methodimpl(std_Logger, findLog,    strc8 name){
	nonull(self, return err);
	nonull(name, return err);
	nonull(self->nameLookup, return err);

return self->nameLookup(name);
}

DESTROY(std_Logger){
	nonull(self, return err);

	del(&priv.logs);
return OK;
}

SIZE(std_Logger){
	nonull(self, return 0);

	return elements ? elements(&priv.logs) : sizeof(std_Logger);
}

ITER(std_Logger){
	nonull(self, return 0);
	
	return index(&priv.logs, index);
}

WRITE(std_Logger){
	nonull(self, return 0);
	
	return std.Type.data.writeTo(varDataOf(&priv.logs), data, size);
}
READ(std_Logger){
	nonull(self, return 0);

	return std.Type.data.readFrom(varDataOf(&priv.logs), data, size);
}
COPY(std_Logger){
	
	std_Logger* dest = where;

	memcpy(where, self, sizeof(std_Logger));

	copy(&priv.logs, &privof(dest).logs);

return where;
}


SET(std_Logger){

	priv.name = value;
	
return OK;
}

construct(std_Logger,
FMT(),
DEF(),
	.Destroy = std_Logger_Op_Destroy,
	.Write   = std_Logger_Op_Write,
	.Read	 = std_Logger_Op_Read,
	.Iter	 = std_Logger_Op_Iter,
	.Size	 = std_Logger_Op_Size,
	.Create  = std_Logger_Op_Create,
	.Set	 = std_Logger_Op_Set,
	.Copy	 = std_Logger_Op_Copy,

){

	nonull(arg.name, return null);

	priv.name = newString(arg.name, 1048);

	if(!priv.name){
		ERR(ERR_FAIL, "failed to duplicate logger name");
	  	return null;
	}
	
	if(create(std_ArrayList, &priv.logs, sizeof(std_Stream*), 10) == null){
	  	ERR(ERR_FAIL, "failed to initialize logs array");
		return null;
	}
	
	 write(&priv.logs, 
	 	arg.infoLog  ? args->infoLog  : null, 
	 	arg.errorLog ? args->errorLog : null
	 );
	
return self;
}



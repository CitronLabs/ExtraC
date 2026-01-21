#include "../Device.h"

static struct {	
	HANDLE handle; 
	len_t lastSize;
	len_t currentSize;
	WORD type;

} StdErr;

errvt moduleFn(Resource_StdErr_open)(streamHandle handle, bool create){
	var StdErrResource = Dev.Resource.getOne(
		WinRTDev.getManager(),
		WinRTDev.getSys(),
		(pntrval)handle
	);

	if(StdErrResource == nil){
		return ERR(ERR.INVALID, "Invalid handle");
	}
	
	StdErr.handle = WinRTCon.getStream(WinRTCon.StreamType.ERR);

	StdErrResource->data = &StdErr;

return OK;
}

errvt moduleFn(Resource_StdErr_close)(streamHandle handle){ 
	return ERR(ERR.INVALID, "Cannot close XC.IO:/Console/StdErr"); 
}

errvt moduleFn(Resource_StdErr_delete)(streamHandle handle){ 
	return ERR(ERR.INVALID, "Cannot delete XC.IO:/Console/StdErr"); 
}

errvt moduleFn(Resource_StdErr_edit)(streamHandle handle, const char* name, word attributes){
	return ERR(ERR.INVALID, "Cannot edit XC.IO:/Console/StdErr"); 
}

errvt moduleFn(Resource_StdErr_watch)(streamHandle handle){

	mod(Resource_StdErr_sync)(handle);
	StdErr.lastSize  	= StdErr.currentSize;

return OK;
}

len_t moduleFn(Resource_StdErr_isModified)(streamHandle handle){
	
	mod(Resource_StdErr_sync)(handle);

return StdErr.currentSize - StdErr.lastSize;
}

len_t moduleFn(Resource_StdErr_shift)(streamHandle handle, word offset, len_t from){
	ERR(ERR.INVALID, "Cannot shift XC.IO:/Console/StdErr"); 
	return 0;
}

len_t moduleFn(Resource_StdErr_readFrom)(streamHandle handle, void* buffer, len_t size){
	ERR(ERR.INVALID, "XC.IO:/Console/StdErr cannot be read from");
	return 0;
}

len_t moduleFn(Resource_StdErr_writeTo)(streamHandle handle, const void* buffer, len_t size){
	DWORD bytesWritten = 0;
	if(!WriteFile(StdErr.handle, buffer, size, &bytesWritten, NULL)){
		ERR(ERR.FAIL, "Failed to write to XC.IO:/Console/StdErr");
		return 0;
	}

return bytesWritten;
}

streamInfo moduleFn(Resource_StdErr_info)(streamHandle handle){
return (streamInfo){
.name 		= "StdErr",
.path 		= "Console/StdErr",
.attributes 	= core.Device.Stream.Attrib.READ,
.type 		= nil,
.currentPos    	= 0,
.time_created  	= 0,
.time_modified 	= 0,
.size 		= StdErr.currentSize
};
}

errvt moduleFn(Resource_StdErr_control)(streamHandle handle, word command, void* args){ return OK; }
errvt moduleFn(Resource_StdErr_flush)(streamHandle handle){ return OK; }
errvt moduleFn(Resource_StdErr_sync)(streamHandle handle){ 
}

#include "../Device.h"

static struct {	
	HANDLE handle; 
	len_t lastSize;
	len_t currentSize;
	WORD type;
} StdIn;

errvt moduleFn(Resource_StdIn_open)(streamHandle handle, bool create){
	var StdInResource = Dev.Resource.getOne(
		WinRTDev.getManager(),
		WinRTDev.getSys(),
		(pntrval)handle
	);

	if(StdInResource == nil){
		return ERR(ERR.INVALID, "Invalid handle");
	}
	
	StdIn.handle = WinRTCon.getStream(WinRTCon.StreamType.INPUT);

	StdInResource->data = &StdIn;

return OK;
}

errvt moduleFn(Resource_StdIn_close)(streamHandle handle){ 
	return ERR(ERR.INVALID, "Cannot close XC.IO:/Console/StdIn"); 
}
errvt moduleFn(Resource_StdIn_delete)(streamHandle handle){ 
	return ERR(ERR.INVALID, "Cannot delete XC.IO:/Console/StdIn"); 
}	
errvt moduleFn(Resource_StdIn_edit)(streamHandle handle, const char* name, word attributes){
	return ERR(ERR.INVALID, "Cannot edit XC.IO:/Console/StdIn"); 
}
errvt moduleFn(Resource_StdIn_watch)(streamHandle handle){

	mod(Resource_StdIn_sync)(handle);
	StdIn.lastSize  	= StdIn.currentSize;

return OK;
}
len_t moduleFn(Resource_StdIn_isModified)(streamHandle handle){
	
	mod(Resource_StdIn_sync)(handle);

return StdIn.currentSize - StdIn.lastSize;
}

len_t moduleFn(Resource_StdIn_shift)(streamHandle handle, word offset, len_t from){
	if(offset < 0){
		ERR(ERR.INVALID, "Cannot rewind XC.IO:/Console/StdIn"); 
		return 0;
	}

	u8 buffer[50] = {};

	if(!ReadFile(StdIn.handle, &buffer, offset, NULL, NULL)){
		 
	}
	

}
len_t moduleFn(Resource_StdIn_readFrom)(streamHandle handle, void* buffer, len_t size){
	DWORD bytesRead = 0;

	if(!ReadFile(StdIn.handle, buffer, size, &bytesRead, NULL)){
		ERR(ERR.FAIL, "Failed to read from XC.IO:/Console/StdIn");
		return 0;
	}
}
len_t moduleFn(Resource_StdIn_writeTo)(streamHandle handle, const void* buffer, len_t size){
	ERR(ERR.INVALID, "XC.IO:/Console/StdIn cannot be written to");
	return 0;
}


streamInfo moduleFn(Resource_StdIn_info)(streamHandle handle){
return (streamInfo){
.name 		= "StdIn",
.path 		= "Console/StdIn",
.attributes 	= core.Device.Stream.Attrib.READ,
.type 		= nil,
.currentPos    	= 0,
.time_created  	= 0,
.time_modified 	= 0,
.size 		= StdIn.currentSize
};
}

errvt moduleFn(Resource_StdIn_control)(streamHandle handle, word command, void* args){ return OK; }
errvt moduleFn(Resource_StdIn_flush)(streamHandle handle){ return OK; }
errvt moduleFn(Resource_StdIn_sync)(streamHandle handle){ 
	StdIn.currentSize;
	return OK; 
}

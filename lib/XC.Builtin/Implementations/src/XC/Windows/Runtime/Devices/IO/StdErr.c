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
errvt moduleFn(Resource_StdErr_isModified)(streamHandle handle){


}

len_t moduleFn(Resource_StdErr_shift)(streamHandle handle, word offset, len_t from){
	if(offset < 0){
		ERR(ERR.INVALID, "Cannot rewind XC.IO:/Console/StdErr"); 
		return 0;
	}

	u8 buffer[50] = {};

	if(!ReadFile(StdErr.handle, &buffer, offset, NULL, NULL)){
		 
	}
	

}
len_t moduleFn(Resource_StdErr_readFrom)(streamHandle handle, void* buffer, len_t size);
len_t moduleFn(Resource_StdErr_writeTo)(streamHandle handle, const void* buffer, len_t size);
streamInfo moduleFn(Resource_StdErr_info)(streamHandle handle){
return (streamInfo){
.name 		= "StdErr",
.path 		= "Console/StdErr",
.attributes 	= XC.Dev.Stream.Attrib.READ,
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
	StdErr.currentSize;
	return OK; 
}

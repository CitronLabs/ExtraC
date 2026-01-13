#include "../Device.h"

static struct IODevice {
	std_Map fileLookup;
	rsrcID 
		stdErrID,
      		stdOutID,
      		stdInID,
      		workDirID;
} IODevice;

static const rsrcInfo 
StdIn_Info = {
.name 		= "StdIn",
.path 		= "Console/StdIn",
.interface 	= &WinRTDev.Resource.StdIn,
.type 		= Dev.Resource.Type.STREAM,
.attributes 	= XC.Dev.Stream.Attrib.READ
},
StdOut_Info = {
.name 		= "StdOut",
.path 		= "Console/StdOut",
.interface 	= &WinRTDev.Resource.StdOut,
.type 		= Dev.Resource.Type.STREAM,
.attributes 	= XC.Dev.Stream.Attrib.WRITE 
},
StdErr_Info = {
.name 		= "StdErr",
.path 		= "Console/StdErr",
.interface 	= &WinRTDev.Resource.StdErr,
.type 		= Dev.Resource.Type.STREAM,
.attributes 	= XC.Dev.Stream.Attrib.WRITE
},
WorkDir_Info = {
.name 		= "WorkDir",
.path 		= "WorkDir",
.interface 	= &WinRTDev.Resource.WorkDir,
.type 		= Dev.Resource.Type.REGISTER,
.attributes 	= XC.Dev.Stream.Attrib.WRITE | XC.Dev.Stream.Attrib.READ
};


static inline pntr moduleFn(IO_Open_Stream)(const char* path, word attributes, void* interface, bool create){

	if(interface != &XC.Dev.Stream.Type.FILE || interface != &XC.Dev.Stream.Type.DIR){
		ERR(ERR.FAIL, "Invalid stream type for XC.IO device, can only FILE or DIR");
		return nil;
	}

	rsrcID  result    = -1;
	rsrcID* foundFile = std.Map.Search(&IODevice.fileLookup, asString(path, PATH_MAX));

	if(foundFile){
		result = *foundFile;

		Dev.Resource.grab(
		    WinRTDev.getManager(),
		    WinRTDev.getIO(),
		    result
		);
	} else {

		rsrcInfo fileInfo = {
			.attributes = attributes,
			.interface  = interface,
			.type 	    = Dev.Resource.Type.STREAM
		};

		rsrcID fileID = Dev.Resource.add(
		    WinRTDev.getManager(),
		    WinRTDev.getIO(),
		    fileInfo,
		    create
		);

		if(fileID == -1){
			ERR(ERR.FAIL, "Failed initialize file resource to IO device");
			return nil;
		}

		Dev.Resource.grab(
		    WinRTDev.getManager(),
		    WinRTDev.getIO(),
		    fileID
		);

		result = fileID;
	}
	
return (pntr)(pntrval)result;
}

static inline errvt moduleFn(IO_InitStdResources)(){
	var devManager = WinRTDev.getManager();
	var IO_DevID   = WinRTDev.getIO();

	IODevice.stdInID = Dev.Resource.add(devManager, IO_DevID, StdIn_Info, true);

	if(IODevice.stdInID == -1)
		return ERR(ERR.INIT, "Failed to initialize Console.StdIn resource");

	IODevice.stdErrID = Dev.Resource.add(devManager, IO_DevID, StdErr_Info, true);

	if(IODevice.stdErrID == -1)
		return ERR(ERR.INIT, "Failed to initialize Console.StdErr resource");

	IODevice.stdOutID = Dev.Resource.add(devManager, IO_DevID, StdOut_Info, true);

	if(IODevice.stdOutID == -1)
		return ERR(ERR.INIT, "Failed to initialize Console.StdOut resource");

	IODevice.workDirID = Dev.Resource.add(devManager, IO_DevID, WorkDir_Info, true);

	if(IODevice.workDirID == -1)
		return ERR(ERR.INIT, "Failed to initialize WorkDir resource");

return OK;
}

pntr moduleFn(IO_Open)(word resource, const char* name, word attributes, void* type){
	nonull(name) return nil;

	switchV(resource){
	caseV(XC.Dev.Resource.Device){
		if(Dev.find(WinRTDev.getManager(), "XC.IO") != -1){
	    		ERR(ERR.INVALID, 
      	    		    "XC.IO device does not "
      	    		    "allow multiple handles "
      	    		    "to itself to be opened"
      	    		);
	    		return nil;
		}

		if(create(std_Map, &IODevice.fileLookup,
			.key  = T(std_String),
	    		.data = T(rsrcID),
	 	) == nil){
			ERR(ERR.INIT, "Failed to create file cache for the XC.IO device");
			return nil;
		}
		
		iferr(mod(IO_InitStdResources)()){
			ERR(ERR.INIT, "Failed to initliaze standard resources for the XC.IO device");
			return nil;
		}

		return &IODevice;
	}
	caseV(XC.Dev.Resource.Stream){ return mod(IO_Open_Stream)(name, attributes, type, true); }
	defaultV {
		ERR(ERR.INVALID, "Invalid resource type");
		return nil;
	}
	}

ERR(ERR.NOTIMPLEM, "unreachable code reached");
return nil;
}

errvt moduleFn(IO_Close)(word resource, pntr handle){
	nonull(handle) return err;

	Manager* devManager 	= WinRTDev.getManager();
	devID 	 IO_DevID 	= WinRTDev.getIO();

	switchV(resource){
	caseV(XC.Dev.Resource.Device){

	    var IODeviceEntry = Dev.getOne(devManager, IO_DevID);

	    loop(i, len(&IODeviceEntry->resources))
	    	Dev.Resource.remove(devManager, IO_DevID, i);		
	    
	    return OK;
	}
	caseV(XC.Dev.Resource.Stream){ 
	    Dev.Resource.release(
	        devManager,
	        IO_DevID,
	        pntr_asVal(handle)
	    );

	    if(!Dev.Resource.usage(devManager, IO_DevID, pntr_asVal(handle)))
	    	Dev.Resource.remove(devManager, IO_DevID, pntr_asVal(handle));

	    return OK;
	}
	defaultV {
		return ERR(ERR.INVALID, "Invalid resource type");
	}
	}

return ERR(ERR.NOTIMPLEM, "unreachable code reached");
}

errvt moduleFn(IO_Edit)(word resourceType, pntr handle, const char* name, word attributes){
	switchV(resourceType){
	caseV(XC.Dev.Resource.Device){
	   return ERR(ERR.INVALID, 
      	    	"XC.IO device is not"
      	    	"allowed to be edited"
      	    );
	}
	caseV(XC.Dev.Resource.Stream){
		var streamInfo = Dev.Resource.getOne(
			WinRTDev.getManager(),
			WinRTDev.getIO(),
			(pntrval)handle
		)->info;

		iferr(((stream_Interface*)streamInfo.interface)
			->edit(handle, name, attributes)
		){ return err; }

		return OK;
	}
	defaultV{
		return ERR(ERR.INVALID, "Invalid resource type");
	}
	}

return ERR(ERR.NOTIMPLEM, "unreachable code reached");
}

pntr moduleFn(IO_Fetch)(word resourceType, const char* name, word attributes, void* type){
	switchV(resourceType){
	caseV(XC.Dev.Resource.Device){
	    ERR(ERR.INVALID, 
      	    	"XC.IO device does not "
      	    	"allow multiple handles "
      	    	"to itself to be opened"
      	    );
	    return nil;
	}
	caseV(XC.Dev.Resource.Stream){ return mod(IO_Open_Stream)(name, attributes, type, false); }
	defaultV{
		ERR(ERR.INVALID, "Invalid resource type");
		return nil;
	}
	}

ERR(ERR.NOTIMPLEM, "unreachable code reached");
return nil;
}
errvt moduleFn(IO_Delete)(word resourceType, pntr handle){
	switchV(resourceType){
	caseV(XC.Dev.Resource.Device){
	   return ERR(ERR.INVALID, 
      	    	"XC.IO device is not"
      	    	"allowed to be deleted"
      	    );
	}
	caseV(XC.Dev.Resource.Stream){

		var streamInfo = Dev.Resource.getOne(
			WinRTDev.getManager(),
			WinRTDev.getIO(),
			(pntrval)handle
		)->info;

		iferr(((stream_Interface*)streamInfo.interface)
			->delete(handle)
		){ return err; }

		return OK;
		

	}
	defaultV{
		return ERR(ERR.INVALID, "Invalid resource type");
	}
	}
return ERR(ERR.NOTIMPLEM, "unreachable code reached");
}

deviceInfo moduleFn(IO_Info)(devHandle handle){

return Dev.getOne(WinRTDev.getManager(), WinRTDev.getIO())->info;
}


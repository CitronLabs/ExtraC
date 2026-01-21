#include "../Device.h"

static struct {
	std_Map envVarLookup;
	rsrcID
       		cliargsID,
       		localeID;
} SysDevice = {};

static const rsrcInfo 
CliArgs_Info = {
.name 		= "CliArgs",
.path 		= "CliArgs",
.interface 	= &WinRTDev.Resource.Locale,
.type 		= Dev.Resource.Type.STREAM,
.attributes 	= core.Device.Stream.Attrib.READ 
},
Locale_Info = {
.name 		= "Locale",
.path 		= "Locale",
.interface 	= &WinRTDev.Resource.CliArgs,
.type 		= Dev.Resource.Type.REGISTER,
.attributes 	= core.Device.Stream.Attrib.READ | core.Device.Stream.Attrib.WRITE
};

static inline pntr moduleFn(Sys_Open_EnvVar)(const char* name, word attributes, void* interface, bool create){

	if(interface != &core.Device.Register.Type.ENV_VAR){
		ERR(ERR.FAIL, "Invalid stream type for XC.IO device, can only FILE or DIR");
		return nil;
	}

	var devManager     = WinRTDev.getManager();
	var IO_DevID       = WinRTDev.getIO();
	rsrcID   result    = -1;
	rsrcID*  foundFile = std.Map.Search(&SysDevice.envVarLookup, asString(name, 255));

	if(foundFile){
		result = *foundFile;

		Dev.Resource.grab(devManager, IO_DevID, result);
	} else {

		rsrcInfo varInfo = {
			.attributes = attributes,
			.interface  = interface,
			.type 	    = Dev.Resource.Type.REGISTER
		};

		rsrcID varID = Dev.Resource.add(devManager, IO_DevID, varInfo);

		if(varID == -1 || Dev.Resource.init(devManager, IO_DevID, varID, create) != OK){
			ERR(ERR.FAIL, "Failed initialize file resource to IO device");
			return nil;
		}

		Dev.Resource.grab(devManager, IO_DevID, varID);

		result = varID;
	}
	
return (pntr)(pntrval)result;

}

static inline errvt moduleFn(Sys_InitStdResources)(){
	var devManager = WinRTDev.getManager();
	var Sys_DevID   = WinRTDev.getSys();

	SysDevice.cliargsID = Dev.Resource.add(devManager, Sys_DevID, CliArgs_Info);

	if(SysDevice.cliargsID == -1 || Dev.Resource.init(devManager, Sys_DevID, SysDevice.cliargsID, true) != OK)
		return ERR(ERR.INIT, "Failed to initialize CliArgs resource");

	SysDevice.localeID = Dev.Resource.add(devManager, Sys_DevID, Locale_Info);

	if(SysDevice.localeID == -1 || Dev.Resource.init(devManager, Sys_DevID, SysDevice.localeID, true) != OK)
		return ERR(ERR.INIT, "Failed to initialize Locale resource");

return OK;
}

pntr moduleFn(Sys_Open)(word resource, const char* name, word attributes, void* type){
	nonull(name) return nil;
	
	Manager* devManager = WinRTDev.getManager();

	switchV(resource){
	caseV(core.Device.Resource.Device){

		if(create(std_Map, &SysDevice.envVarLookup,
			.key  = T(std_String),
	    		.data = T(rsrcID),
	 	) == nil){
			ERR(ERR.INIT, "Failed to create file cache for the XC.IO device");
			return nil;
		}
		
		iferr(mod(Sys_InitStdResources)()){
			ERR(ERR.INIT, "Failed to initliaze standard resources for the XC.IO device");
			return nil;
		}

		return &SysDevice;
	}
	caseV(core.Device.Resource.Register){ return mod(Sys_Open_EnvVar)(name, attributes, type, true); }
	defaultV{
		ERR(ERR.INVALID, 
      			"XC.Sys device does not allow "
      			"creation, deletion, or modification "
      			"of any streams by users"
		);
		return nil;

	}
	}

ERR(ERR.NOTIMPLEM, "unreachable code reached");
return nil;
}


errvt moduleFn(Sys_Close)(word resource, pntr handle){
	switchV(resource){
	caseV(core.Device.Resource.Device){
		return ERR(ERR.INVALID, 
      			"XC.Sys device does not allow "
      			"creation, deletion, or modification "
      			"itself by users"
		);
	}
	caseV(core.Device.Resource.Register){
		iferr(Dev.Resource.remove(
			WinRTDev.getManager(),
			WinRTDev.getSys(),
			pntr_asVal(handle)
		)){
		    return ERR(ERR.FAIL, 
		 	"Failed to close the "
		 	"environment variable resource"
		    );
		}

		return OK;
	}
	defaultV{
		return ERR(ERR.INVALID, 
      			"XC.Sys device does not allow "
      			"creation, deletion, or modification "
      			"of any streams by users"
		);
	}
	}
return ERR(ERR.NOTIMPLEM, "unreachable code reached");
}

errvt moduleFn(Sys_Edit)(word resourceType, pntr handle, const char* name, word attributes){
	switchV(resourceType){
	caseV(core.Device.Resource.Device){
		return ERR(ERR.INVALID, 
      			"XC.Sys device does not allow "
      			"creation, deletion, or modification "
      			"itself by users"
		);
	}
	caseV(core.Device.Resource.Register){
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
		return ERR(ERR.INVALID, 
      			"XC.Sys device does not allow "
      			"creation, deletion, or modification "
      			"of any streams by users"
		);
	}
	}
return ERR(ERR.NOTIMPLEM, "unreachable code reached");
}

pntr moduleFn(Sys_Fetch)(word resourceType, const char* name, word attributes, void* type){
	switchV(resourceType){
	caseV(core.Device.Resource.Device){
	    ERR(ERR.INVALID, 
      	    	"XC.Sys device does not "
      	    	"allow multiple handles "
      	    	"to be opened"
      	    );
	    return nil;
	}
	caseV(core.Device.Resource.Register){ return mod(Sys_Open_EnvVar)(name, attributes, type, false); }
	defaultV{
		ERR(ERR.INVALID, 
      			"XC.Sys device does not allow "
      			"creation, deletion, or modification "
      			"of any streams by users"
		);
		return nil;
	}
	}
ERR(ERR.NOTIMPLEM, "unreachable code reached");
return nil;
}

errvt moduleFn(Sys_Delete)(word resourceType, pntr handle){
	switchV(resourceType){
	caseV(core.Device.Resource.Device){
		return ERR(ERR.INVALID, 
      			"XC.Sys device does not allow "
      			"creation, deletion, or modification "
      			"itself by users"
		);
	}
	caseV(core.Device.Resource.Register){
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
		return ERR(ERR.INVALID, 
      			"XC.Sys device does not allow "
      			"creation, deletion, or modification "
      			"of any streams by users"
		);
	}
	}
return ERR(ERR.NOTIMPLEM, "unreachable code reached");
}

deviceInfo moduleFn(Sys_Info)(devHandle handle){
return Dev.getOne(WinRTDev.getManager(), WinRTDev.getSys())->info;
}





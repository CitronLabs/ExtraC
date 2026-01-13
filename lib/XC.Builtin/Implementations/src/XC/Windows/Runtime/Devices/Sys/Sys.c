#include "../Device.h"

static struct {
	rsrcID
       		cliargsID,
       		localeID;
} SysDevice = {};

pntr moduleFn(Sys_Open)(word resource, const char* name, word attributes){
	nonull(name) return nil;
	
	Manager* devManager = WinRTDev.getManager();

	switchV(resource){
	caseV(XC.Dev.Resource.Device){

	   	SysDevice.cliargsID =
	   	     Dev.Resource.add(
	   	     	devManager,
	   	     	Sys_DevID,
	   	     	Sys_CliArgs_InterfaceObj
	   	 	);

	   	SysDevice.localeID =
	   	     Dev.Resource.add(
	   	     	devManager,
	   	     	Sys_DevID,
	   	     	Sys_Locale_InterfaceObj
	   	 	);

	   	if(
	   	     SysDevice.cliargsID == -1 ||
	   	     SysDevice.localeID  == -1
       	   	){
	   	     ERR(ERR.INIT, 
      	   	     	RED"[CRITICAL] Failed to initialize "
      	   	     	    "XC.Sys device's standard resources"
      	   	     );
	   	     return nil;
	   	}

	   	return &SysDevice;
	}
	caseV(XC.Dev.Resource.Register){
		rsrcInterface_Obj envVar = {
			&Sys_EnvVar_ResourceInterface,
			nil
		};

		rsrcID envVarID = Dev.Resource.add(
					devManager,
					Sys_DevID,
					envVar
				);

		return (pntr)(pntrval)envVarID;

	}
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
	caseV(XC.Dev.Resource.Device){
		return ERR(ERR.INVALID, 
      			"XC.Sys device does not allow "
      			"creation, deletion, or modification "
      			"itself by users"
		);
	}
	caseV(XC.Dev.Resource.Register){
		iferr(Dev.Resource.remove(
			WinRTDev.getManager(),
			Sys_DevID,
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
	caseV(XC.Dev.Resource.Device){
		return ERR(ERR.INVALID, 
      			"XC.Sys device does not allow "
      			"creation, deletion, or modification "
      			"itself by users"
		);
	}
	caseV(XC.Dev.Resource.Register){
		
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

pntr moduleFn(Sys_Fetch)(word resourceType, const char* name, word attributes){
	switchV(resourceType){
	caseV(XC.Dev.Resource.Device){
	    ERR(ERR.INVALID, 
      	    	"XC.Sys device does not "
      	    	"allow multiple handles "
      	    	"to be opened"
      	    );
	    return nil;
	}
	caseV(XC.Dev.Resource.Register){


	}
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
	caseV(XC.Dev.Resource.Device){
		return ERR(ERR.INVALID, 
      			"XC.Sys device does not allow "
      			"creation, deletion, or modification "
      			"itself by users"
		);
	}
	caseV(XC.Dev.Resource.Register){
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
return (deviceInfo){
	.name 	     = Sys_DevInterface.info.name,
	.productName = Sys_DevInterface.info.productName,
	.vendorName  = Sys_DevInterface.info.vendorName,
	.serialCode  = Sys_DevInterface.info.serialCode,
	.path  	     = Sys_DevInterface.info.devPath,
	.attributes  = XC.Dev.Attrib.PRIVATE,
	.num_registers  = 3,
	.num_streams    = 0,
	.num_resources  = 3,
	.valid    	= true
};
}





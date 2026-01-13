#include "DevicesManager.h"
#define module env, Common, Devices



ifob(env_Common_Device)* moduleMethod(devManager, getOne, devID id){
	nonull(self){ return nil; }
	if(!id)	    { nullerr(id); return nil; }

	Device* dev = index(&priv.registeredDevices, id);
	
	if(!dev){
		ERR(ERR.FAIL, "failed to find device from the device ID");
		return nil;
	}

return generic &dev->data;
}

errvt moduleMethod(devManager, getAll, std_Array_Buffer* envDevices){
	nonull(self, envDevices){ return err; }

	foreach(&priv.registeredDevices, Device, dev)
		write(envDevices, &dev->data);
	
return OK;
}

devID moduleMethod(devManager, add, ifob(env_Common_Device) data){
	
	Device dev = {
		.isConnected = true,
	};

	memcpy(&data, &dev.data, sizeof(ifob(env_Common_Device)));

	devID result = std.List.FillSlot(&priv.registeredDevices, &dev);

return result;
}

devID moduleMethod(devManager, find, strc8 path){
	nonull(path){ return -1; }

	devID* result = std.Map.Search(
		&priv.pathResolve, 
		asString(path, sizeof(std_FSPath))
	);

	if(result == nil){
		return -1;
	}

return *result;
}
errvt moduleMethod(devManager, remove, devID id){ 
	nonull(self){ return err; }

	Device* dev = index(&priv.registeredDevices, id);
	
	if(!dev)
		return ERR(ERR.FAIL, "failed to find device from the device ID");

	if(dev->isConnected || dev->numReferences != 0){
		return ERR(ERR.BUSY, "device is still in use");
	}

	foreach(&dev->resources, resourceData, resource){
		i(resource).onExit(resource_iterator);
	}

}

bool  moduleMethod(devManager, isConnected, devID id)
	{ return ((Device*)index(&priv.registeredDevices, id))->isConnected;}

errvt moduleMethod(devManager, disconnect, devID id){
	nonull(self){ return err; }

	Device* dev = index(&priv.registeredDevices, id);
	
	if(!dev)
		return ERR(ERR.FAIL, "failed to find device from the device ID");
	
	
	dev->isConnected = false;

return OK;
}
errvt moduleMethod(devManager, grab, devID id){
	nonull(self){ return err; }
	if(!id)	    { return nullerr(id); }


	Device* dev = index(&priv.registeredDevices, id);
	
	if(!dev)
		return ERR(ERR.FAIL, "failed to find device from the device ID");
	
	
	dev->numReferences++;
return OK;
}

errvt moduleMethod(devManager, release, devID id){
	nonull(self){ return err; }
	if(!id)	    { return nullerr(id); }

	Device* dev = index(&priv.registeredDevices, id);
	
	if(!dev)
		return ERR(ERR.FAIL, "failed to find device from the device ID");
	
	
	dev->numReferences--;
return OK;
}

	
construct(env_Common_Devices_Manager,
FMT(),
DEF(),
	
){
	create(List, &priv.registeredDevices,
		.type 	   	= cT(Device),
		.init_size 	= 10
	);


	create(Map, &priv.pathResolve,
		.key  		= T(String),
		.data 	 	= T(devID),
		.init_size 	= 5
	);

return self;
};



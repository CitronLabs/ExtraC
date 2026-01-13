#include "DevicesManager.h"
#define module env, Common, Devices, Manager, Resource





resourceID moduleMethod(devManager, add, devID id, resourceData resource){
	nonull(self){ return DEVICE_ID_INVALID; }


	Device* dev = index(&priv.registeredDevices, id);

	if(dev == nil){
		ERR(ERR.INVALID, "invalid device ID");

		return DEVICE_ID_INVALID;
	}

	resourceID result = std.List.FillSlot(&dev->resources, &resource); check(){
		ERR(ERR.FAIL, "failed to register resource for device");

		return DEVICE_ID_INVALID;
	}

	iferr(i(&resource).onInit(result)){
		ERR(ERR.INIT, "error occured when initializing resource");
		std.List.SetFree(&dev->resources, result);

		return DEVICE_ID_INVALID;
	}

return result;
}

const resourceIntf* moduleMethod(devManager, getInterface, devID id, resourceID resource){



}

errvt moduleMethod(devManager, remove, devID id, resourceID resource){
	nonull(self) { return err; }

	Device* dev = index(&priv.registeredDevices, id);
	
	if(!dev)
		return ERR(ERR.FAIL, "failed to find device from the device ID");

return OK;





}

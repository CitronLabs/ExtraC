#include <XC.pkg.c>


from(env_Common_Devices, 
    Manager				as devManager,
    ID      				as devID,
    Manager_Resource_ID      		as resourceID,
    Manager_Resource_Interface 		as resourceIntf,
    Manager_Resource_Interface_Obj 	as resourceData,
);

from(std,
	use(Map),
	use(String),
	use(List),
);


typedef struct {
	List resources;
	len_t numReferences;
	ubyte isConnected : 1;
	
	ifob(env_Common_Device) data;
} Device;

alias(env.Common.Devices.Manager.Resource, Resource);

defTypeID(env_Common_Device_ID);
defTypeID(env_Common_Device_Data);

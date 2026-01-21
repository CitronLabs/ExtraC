#pragma once
#define __ENV_COMMON_DEVICES__
#include "../pkg.h"
#include "../types.h"


#ifndef __ENV_COMMON_DEVICES_MANAGER__
#define package env_Common_Devices


Class(Manager,
INIT(),
FMT(),


private(
	List(Entry)  	registeredDevices;
      	Map(String, ID)	pathResolve;
)
){
	pkg(Entry)* 	method(Manager, getOne, 	pkg(ID) id);
	errvt 	  	method(Manager, getAll, 	std_Array_Buffer* envDevices);
	pkg(ID) 	method(Manager, add,   		deviceInfo info, intf(device) type);
	errvt 		method(Manager, init,   	pkg(ID) id, bool create);
	bool 		method(Manager, isInit,   	pkg(ID) id);

	pkg(ID) 	method(Manager, find, 		strc8 path);
	errvt 	  	method(Manager, remove, 	pkg(ID) id);
	bool 	  	method(Manager, isConnected, 	pkg(ID) id);
	errvt 	  	method(Manager, disconnect, 	pkg(ID) id);
	errvt 	  	method(Manager, grab, 		pkg(ID) id);
	errvt 	  	method(Manager, release, 	pkg(ID) id);

  submodule(Resource,
	values(Type, pkg(ResourceType),
		STREAM,
		REGISTER
	)
	errvt 		method(Manager, grab, 	 pkg(ID) id, pkg(ResourceID) resource);
	errvt 		method(Manager, release, pkg(ID) id, pkg(ResourceID) resource);
	len_t 		method(Manager, usage,   pkg(ID) id, pkg(ResourceID) resource);

	pkg(ResourceID)	method(Manager, add,     pkg(ID) id, pkg(ResourceInfo) info);
	errvt 		method(Manager, init,    pkg(ID) id, pkg(ResourceID) resource, bool create);
	pkg(Resource)* 	method(Manager, getOne,  pkg(ID) id, pkg(ResourceID) resource);

	errvt		method(Manager, remove,	 pkg(ID) id, pkg(ResourceID) resource);
  )
};

#undef package
#endif

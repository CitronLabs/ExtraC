#pragma once
#define __ENV_COMMON_DEVICES__
#include "pkg.h"

#define package env_Common_Devices


typefrom(word, ID);

#define DEVICE_ID_INVALID -1

typefrom(word, ResourceID);
typefrom(word, ResourceType);

type(ResourceInfo,
     	pkg(ResourceType) 	type;
	const c8 		*name, *path;
	len_t 	  		size,  currentPos;
	int 	  		attributes;
     	const void* 		interface;
)

type(Resource,
	pkg(ResourceInfo) 	info;
     	pntr 			data;
)

type(Entry,
	deviceInfo 		info;
     	intf(device) 		interface;
     	Array(pkg(Resource))	resources;
     	pntr 			data;
)

#undef package

#pragma once
#include "../types.h"
#include "./Stream.h"
#include "./Register.h"


typedef struct deviceInfo {
	const c8
     	* name,
     	* vendorName,
     	* productName,
     	* serialCode,
     	* path;

	int attributes;

     	len_t 
	    num_streams, 
	    num_registers, 
	    num_resources;

	bool valid;
} deviceInfo;

Interface(device,
	errvt 	   fn(edit)	    (word resourceType, pntr handle, const char* name, word attributes);	
	errvt 	   fn(close)	    (word resourceType, pntr handle);
	pntr 	   fn(fetch)	    (word resourceType, const char* name, word attributes, void* type);
	pntr       fn(open) 	    (word resourceType, const char* name, word attributes, void* type);
	errvt 	   fn(delete)	    (word resourceType, pntr handle);
	deviceInfo fn(info) 	    (devHandle handle); 
);


Interface(__XC_DEVICE,
	values(Attrib, word,
		PUBLIC,
		PRIVATE,
		REGISTER_CREATE,
		STREAM_CREATE
	)
	values(ID, word,
		Sys,
		Local,
		IO
	)
	values(Resource, word,
		Self,
		Device,
		Stream,
		Register
	)
	devHandle fn(stdHandle)(word id); 
	devHandle fn(open)(const char* name, word attributes, device_Interface* type);
	devHandle fn(fetch)(const char* name);

	deviceInfo fn(info)(devHandle handle); 
	errvt 	   fn(close)(devHandle handle);

	submodule(Type,
		interface(device) VIRTUAL;
	)

	interface(__XC_DEV_REGISTER) Register;
	interface(__XC_DEV_STREAM)   Stream;

)

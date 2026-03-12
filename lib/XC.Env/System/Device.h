#pragma once
#include "../types.h"


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

typedef const struct device_Interface {
	const void* type;
	pntr       (* const init) 	    (devHandle handle);
	errvt 	   (* const close)	    (devHandle handle);
	errvt 	   (* const delete)	    (devHandle handle);
	deviceInfo (* const info) 	    (devHandle handle); 
} device_Interface;


typedef const struct __XC_DEVICE_Interface {

	devHandle (* const init)(const char* path, const char* name, device_Interface type);
	devHandle (* const fetch)(const char* path, const char* name);

	deviceInfo (* const info)(devHandle handle); 
	errvt 	   (* const close)(devHandle handle);

} __XC_DEVICE_Interface;

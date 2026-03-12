#pragma once
#include "../types.h"

typedef struct registerInfo {
	len_t 		size, 
			time_created, 
			time_modified, 
			currentPos;

	const char	* name,
			* path;

	int 		  attributes;

	const void* 	type;

	bool 		valid;

} registerInfo;



typedef struct register_Interface {
	const void* type;
	pntr (* const open)(devHandle dev, strc8 name, word attributes, bool create);	

	errvt (* const close)(registerHandle handle);
	errvt (* const delete)(registerHandle handle);

	errvt (* const edit)(registerHandle handle, const char* name, word attributes);
	errvt (* const watch)(registerHandle handle);
	len_t (* const isModified)(registerHandle handle);

	len_t (* const readFrom)(registerHandle handle, const void* buffer, len_t size);
	len_t (* const writeTo) (registerHandle handle, const void* buffer, len_t size);
	len_t (* const setTo)   (registerHandle handle, const void* buffer, len_t size);
	void* (* const access)  (registerHandle handle);

	registerInfo (* const info)(registerHandle handle);
} register_Interface;


typedef struct __XC_REGISTER_Interface {

	struct {
	    const word
	    	READ,
	    	WRITE,
	    	DIRECT_ACCESS;
	} Attrib;

	registerHandle (* const open)(devHandle dev, strc8 name, word attributes, register_Interface interface);	
	registerHandle (* const fetch)(devHandle dev, strc8 name);

	errvt (* const close)(registerHandle handle);
	errvt (* const delete)(registerHandle handle);

	errvt (* const edit)(registerHandle handle, const char* name, word attributes);
	errvt (* const watch)(registerHandle handle);
	len_t (* const isModified)(registerHandle handle);

	len_t (* const readFrom)(registerHandle handle, const void* buffer, len_t size);
	len_t (* const writeTo) (registerHandle handle, const void* buffer, len_t size);
	len_t (* const setTo)   (registerHandle handle, const void* buffer, len_t size);
	void* (* const access)  (registerHandle handle);

	registerInfo (* const info)(registerHandle handle);

} __XC_REGISTER_Interface;

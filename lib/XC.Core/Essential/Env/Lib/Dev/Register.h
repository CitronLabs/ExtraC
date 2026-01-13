#pragma once
#include "../types.h"


typedef const struct register_Interface register_Interface;

typedef struct registerInfo {
	len_t 		size, 
			time_created, 
			time_modified, 
			currentPos;

	const char	* name,
			* path;

	int 		  attributes;

	register_Interface  * type;

	bool 		  valid;

} registerInfo;



Interface(register,
	errvt fn(open)(registerHandle handle, bool create);	
	errvt fn(close)(registerHandle handle);	
	errvt fn(delete)(registerHandle handle);	
	errvt fn(edit)(registerHandle handle, const char* name, word attributes);
	errvt fn(watch)(registerHandle handle);
	errvt fn(isModified)(registerHandle handle);

	len_t fn(readFrom)(registerHandle handle, const void* buffer, len_t size);
	len_t fn(writeTo) (registerHandle handle, const void* buffer, len_t size);
	len_t fn(setTo)   (registerHandle handle, const void* buffer, len_t size);
	void* fn(access)  (registerHandle handle);

	registerInfo fn(info)(registerHandle handle);
)


Interface(__XC_DEV_REGISTER,
	values(Attrib, word,
	    READ,
	    WRITE,
	    DIRECT_ACCESS
	)

	values(ID, word,
		CliArgs,
		Locale,
		WorkDir
	)

	registerHandle fn(stdHandle)(word id); 
	registerHandle fn(open)(devHandle dev, strc8 name, len_t size, word attributes, register_Interface* type);	
	registerHandle fn(fetch)(devHandle dev, word id);

	errvt fn(close)(registerHandle handle);
	errvt fn(delete)(registerHandle handle);


	interface(register) Modify;

	submodule(Type,
		interface(register) GENERIC;
	)
)

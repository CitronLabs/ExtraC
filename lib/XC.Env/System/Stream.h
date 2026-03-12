#pragma once
#include "../types.h"

typedef struct streamInfo {
	len_t 		size, 
			time_created, 
			time_modified, 
			currentPos;

	const char	* name,
			* path;

	int 		  attributes;

	const void* 	type;

	bool 		valid;

} streamInfo;


typedef const struct stream_Interface {
	const void* type;
	pntr  (* const open)(devHandle dev, const char* key, word attributes, bool create);	

	errvt (* const close)(streamHandle handle);
	errvt (* const delete)(streamHandle handle);

	errvt (* const edit)(streamHandle handle, const char* name, word attributes);
	errvt (* const watch)(streamHandle handle);
	len_t (* const isModified)(streamHandle handle);

	len_t (* const shift)(streamHandle handle, word offset, len_t from);
	len_t (* const readFrom)(streamHandle handle, void* buffer, len_t size);
	len_t (* const writeTo)(streamHandle handle, const void* buffer, len_t size);
	streamInfo (* const info)(streamHandle handle);

 	errvt (* const control)(streamHandle handle, word command, void* args); 
        errvt (* const flush)(streamHandle handle); 
        errvt (* const sync)(streamHandle handle); 
} stream_Interface;

typedef const struct __XC_STREAM_Interface {
	struct { const word READ, WRITE; } Attrib;

	streamHandle (* const open)(devHandle dev, const char* key, word attributes, stream_Interface type);	

	streamHandle (* const fetch)(devHandle dev, const char* key, word attributes); 	
	
	errvt (* const close)(streamHandle handle);
	errvt (* const delete)(streamHandle handle);

	errvt (* const edit)(streamHandle handle, const char* name, word attributes);
	errvt (* const watch)(streamHandle handle);
	len_t (* const isModified)(streamHandle handle);

	len_t (* const shift)(streamHandle handle, word offset, len_t from);
	len_t (* const readFrom)(streamHandle handle, void* buffer, len_t size);
	len_t (* const writeTo)(streamHandle handle, const void* buffer, len_t size);
	streamInfo (* const info)(streamHandle handle);

 	errvt (* const control)(streamHandle handle, word command, void* args); 
        errvt (* const flush)(streamHandle handle); 
        errvt (* const sync)(streamHandle handle); 

} __XC_STREAM_Interface;

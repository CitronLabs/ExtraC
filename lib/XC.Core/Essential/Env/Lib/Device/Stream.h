#pragma once
#include "../types.h"

typedef const struct stream_Interface stream_Interface;

typedef struct streamInfo {
	len_t 		size, 
			time_created, 
			time_modified, 
			currentPos;

	const char	* name,
			* path;

	int 		  attributes;

	stream_Interface* type;

	bool 		  valid;

} streamInfo;


Interface(stream,
	errvt fn(open)(streamHandle handle, bool create);	
	errvt fn(close)(streamHandle handle);	
	errvt fn(delete)(streamHandle handle);	
	errvt fn(edit)(streamHandle handle, const char* name, word attributes);
	errvt fn(watch)(streamHandle handle);
	len_t fn(isModified)(streamHandle handle);

	len_t fn(shift)(streamHandle handle, word offset, len_t from);
	len_t fn(readFrom)(streamHandle handle, void* buffer, len_t size);
	len_t fn(writeTo)(streamHandle handle, const void* buffer, len_t size);
	streamInfo fn(info)(streamHandle handle);

 	errvt fn(control)(streamHandle handle, word command, void* args); // Generic IOCTL/FCNTL abstraction
        errvt fn(flush)(streamHandle handle); // Forces pending writes to the underlying medium
        errvt fn(sync)(streamHandle handle); // Ensures data and metadata are written (fsync)
);

Interface(__XC_DEV_STREAM,
	values(Attrib, word,
	    READ, WRITE
	)

	values(ID, word,
		In, Out, Err
	)

	streamHandle fn(stdHandle)(word id); 

	// open can be used to create a stream as well as grabbing the streamHandle
	streamHandle fn(open)(devHandle dev, const char* key, word attributes, stream_Interface* type);	

	// fetch can be used to check if a stream exists as well as grabbing the streamHandle
	streamHandle fn(fetch)(devHandle dev, const char* key, word attributes); 	
	
	errvt fn(edit)(streamHandle handle, const char* key, word attributes);	
	errvt fn(close)(streamHandle handle);
	errvt fn(delete)(streamHandle handle);

	interface(stream) Modify;

	submodule(Type,
		interface(stream) FILE;
		interface(stream) DIR;
	)
)

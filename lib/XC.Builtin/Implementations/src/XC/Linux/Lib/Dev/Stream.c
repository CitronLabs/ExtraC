#pragma once
#include <XC/pkg.c>

#define module XC, Dev, Stream

from(env_Common,
	Devices_ID 		as devID,
     	Devices_ResourceData 	as StreamResource,
     	Posix_IO_Dir		as Dir,
     	Posix_IO_File		as File
);

alias(env.Linux.Runtime.Devices,   Devices);
alias(env.Common.Devices.Resource, Resource);
alias(XC.Dev.Stream.Attrib,        Attribs);

StreamResource openIOStream(const char* key, word attributes, bool create){
	
	StreamResource stream = {0};

	if(getbitflag(attributes, Attribs.DIR)){
	    stream.interface = &env.Common.Posix.IO.Dir.Stream;

	    stream.object = new(Dir,
	    	.path     = key,
	    	.flags    = {
	    	    .create = create,
	    	    .read   = getbitflag(attributes, Attribs.READ),
	    	    .write  = getbitflag(attributes, Attribs.WRITE),
	    	 }
	    );

	} else {
	    stream.interface = &env.Common.Posix.IO.File.Stream;

	    stream.object 	= new(File,
	    	.path   = key,
	    	.flags  = {
	    	    .create = create,
	    	    .read   = getbitflag(attributes, Attribs.READ),
	    	    .write  = getbitflag(attributes, Attribs.WRITE),
	    	 }
	    );

	}

return stream;
}

streamHandle moduleFn(open)(
	devHandle dev, 
	const char* key, 
	word attributes, 
	streamSettings* settings
){
	nonull(dev, key){ return nil; }

	StreamResource stream;

	if(pntr_asVal(dev) == Devices.getIO())
		stream = openIOStream(key, attributes, true);
	
	if(stream.object == nil){
		ERR(ERR.FAIL, "failed to create stream object");
		return nil;
	}

return (streamHandle)(len_t)
	Resource.add(
		Devices.getManager(),
		pntr_asVal(dev),
		Resource.Type.STREAM,
		stream
	);
}

// fetch can be used to check if a stream exists as well as grabbing the streamHandle
streamHandle moduleFn(fetch)(
	devHandle dev, 
	const char* key, 
	word attributes, 
	streamSettings* settings
){
	nonull(dev, key){ return nil; }

	StreamResource stream;

	if(pntr_asVal(dev) == Devices.getIO())
		stream = openIOStream(key, attributes, false);
	
	if(stream.object == nil){
		if(errnm != ERR.INVALID)
			ERR(ERR.FAIL, "failed to fetch stream object");

		return nil;
	}

return (streamHandle)(len_t)
	Resource.add(
		Devices.getManager(),
		pntr_asVal(dev),
		Resource.Type.STREAM,
		stream
	);
}

errvt moduleFn(watch)(streamHandle handle);
errvt moduleFn(isModified)(streamHandle handle);
errvt moduleFn(modify)(streamHandle handle, const char* key, word attributes);	
errvt moduleFn(drop)(streamHandle handle);
errvt moduleFn(close)(streamHandle handle);

streamHandle moduleFn(stdHandle)(word id); // For stdin/stdout/stderr

len_t moduleFn(shift)(streamHandle handle, word offset, len_t from);
len_t moduleFn(readFrom)(streamHandle handle, void* buffer, len_t size);
len_t moduleFn(writeTo)(streamHandle handle, const void* buffer, len_t size);
streamInfo moduleFn(info)(streamHandle handle);

errvt moduleFn(control)(streamHandle handle, word command, void* args); // Generic IOCTL/FCNTL abstraction
errvt moduleFn(flush)(streamHandle handle); // Forces pending writes to the underlying medium
errvt moduleFn(sync)(streamHandle handle); // Ensures data and metadata are written (fsync)

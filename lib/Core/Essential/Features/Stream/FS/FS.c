#include "../../../../pkg.h"

import(std)

import(XC)

enum {
	__IO_CREATE_VALUE = 1, 
	__IO_APPEND_VALUE = 2, 
	__IO_WRITE_VALUE  = 3,
	__IO_READ_VALUE   = 4
};

std_Stream* std_IO_open(std_FSPath path, int flags){
	devHandle io_dev = XC.Dev.stdHandle(XC.Dev.ID.IO);
	
	streamHandle strm = XC.Dev.Stream.open(io_dev, path, flags, null);

return new(std_Stream, std.Stream.Preset.fromHandle(strm));
}
errvt std_IO_delete(std_FSPath path){
	devHandle io_dev = XC.Dev.stdHandle(XC.Dev.ID.IO);
	
	streamHandle strm = XC.Dev.Stream.open(io_dev, path, 0, null);

	XC.Dev.Stream.close(strm);
return OK;
}
errvt std_IO_chdir(std_FSPath path){
	devHandle io_dev = XC.Dev.stdHandle(XC.Dev.ID.IO);
	
	registerHandle curr_dir = XC.Dev.Register.fetch(io_dev, 0);

	XC.Dev.Register.writeTo(curr_dir, path, strnlen(path, sizeof(std_FSPath)));
return OK;
}
std_Stream* std_IO_search(std_FSPath path, std_FSEntry* ent){
	devHandle io_dev = XC.Dev.stdHandle(XC.Dev.ID.IO);
	
	streamHandle strm = XC.Dev.Stream.fetch(io_dev, path, 0, null);
	
	if(!strm) {
		return nil;
	}

	streamInfo info = XC.Dev.Stream.info(strm);

	*ent = (std_FSEntry){
	    .type.is.dir   = getbitflag(info.attributes, XC.Dev.Stream.Attrib.DIR),
	    .type.is.link  = getbitflag(info.attributes, XC.Dev.Stream.Attrib.LINK),
	    .name 	   = generic info.name,
	    .path 	   = copy_use(std_FSPath_Type, &path, new_alloc(std_FSPath)),
	    .size 	   = info.size,
	    .time_created  = info.time_created,
	    .time_modified = info.time_modified,
	    .stream 	   = new(std_Stream, std.Stream.Preset.fromHandle(strm)),
	    .__type 	   = std_FSEntry_Type,
	};

return ent->stream;
}


errvt std_FS_setname(std_FSPath path, std_FSPath new_name){
	devHandle io_dev = XC.Dev.stdHandle(XC.Dev.ID.IO);
	
	streamHandle strm = XC.Dev.Stream.open(io_dev, path, 0, null);

	XC.Dev.Stream.modify(strm, new_name, 0);

return OK;
}

READ(std_FSEntry){
	nonull(self, return 0);

return std.Types.data.readFrom(
  	VFrom(streamHandle_Type, std.Stream.getHandle(self->stream)),
  	data, size
);
}
WRITE(std_FSEntry){
	nonull(self, return 0);

return std.Types.data.writeTo(
  	VFrom(streamHandle_Type, std.Stream.getHandle(self->stream)),
  	data, size
);
}
SET(std_FSEntry){
return std.FS.setname(self->path, value);
}
COPY(std_FSEntry){
	memcpy(where, self, sizeof(std_FSEntry));
return where;
}
DESTROY(std_FSEntry){
	del(self->stream);
	free(self->path);
return OK;
}
HASH(std_FSEntry){
	return hash_bytes(
	  	self->path, 
	  	strnlen(self->path, sizeof(std_FSPath))
	);
}
SIZE(std_FSEntry){
	if(!self)
	  return sizeof(std_FSEntry);
	else 
	  return self->size;
}

ITER(std_FSEntry){
	return index(self->stream, index);
}

construct(std_FSEntry,
FMT(),
DEF(),
	.Print = nilmethod, // planned for next release
	.Scan  = nilmethod, // planned for next release
	
){ 
	if(!std.FS.open(arg.path, args->flags, self)){
		ERR(ERR_FAIL, "failed to open FS entry");
	  	return nil;
	}

return self;
}
 


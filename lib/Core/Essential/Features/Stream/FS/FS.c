#include "../../../../pkg.h"

import(std)
import(XC)

#define module std, FS

std_Stream* moduleFn(open)(std_FSPath path, int flags){
	devHandle io_dev = XC.Dev.stdHandle(XC.Dev.ID.IO);
	
	streamHandle strm = XC.Dev.Stream.open(io_dev, path, flags, nil);

return new(std_Stream, std.Stream.Preset.fromHandle(strm));
}
errvt moduleFn(delete)(std_FSPath path){
	devHandle io_dev = XC.Dev.stdHandle(XC.Dev.ID.IO);
	
	streamHandle strm = XC.Dev.Stream.open(io_dev, path, 0, nil);

	XC.Dev.Stream.close(strm);
return OK;
}
errvt moduleFn(chdir)(std_FSPath path){
	devHandle io_dev = XC.Dev.stdHandle(XC.Dev.ID.IO);
	
	registerHandle curr_dir = XC.Dev.Register.fetch(io_dev, 0);

	XC.Dev.Register.writeTo(curr_dir, path, strnlen(path, sizeof(std_FSPath)));
return OK;
}
std_Stream* moduleFn(search)(std_FSPath path, std_FSEntry* ent){
	devHandle io_dev = XC.Dev.stdHandle(XC.Dev.ID.IO);
	
	streamHandle strm = XC.Dev.Stream.fetch(io_dev, path, 0, nil);
	
	if(strm == nil) {
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


errvt moduleFn(setname)(std_FSPath path, std_FSPath new_name){
	devHandle io_dev = XC.Dev.stdHandle(XC.Dev.ID.IO);
	
	streamHandle strm = XC.Dev.Stream.open(io_dev, path, 0, nil);

	XC.Dev.Stream.modify(strm, new_name, 0);

return OK;
}

READ(std_FSEntry){
	nonull(self){ return 0; }

return std.Types.data.readFrom(
  	VFrom(streamHandle_Type, std.Stream.getHandle(self->stream)),
  	data, size
);
}
WRITE(std_FSEntry){
	nonull(self){ return 0; }

return std.Types.data.writeTo(
  	VFrom(streamHandle_Type, std.Stream.getHandle(self->stream)),
  	data, size
);
}
SET(std_FSEntry){
return std.FS.rename(self->path, value);
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
	if(std.FS.open(arg.path, args->flags, self) == nil){
		ERR(ERR_FAIL, "failed to open FS entry");
	  	return nil;
	}

return self;
}
 


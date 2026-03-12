#include "../FS.h"
#include "Async.c"
#define module std, FS, File

errvt moduleFn(manualCopy)(devHandle io_dev, streamHandle from, streamHandle to){ 
	u64 bytes_to_transfer = 0, cursor_offset = 0;

	for(;;){
		u8 buff[1028];

		bytes_to_transfer = Stream.Modify.readFrom(from, buff, 1028);

		if(!bytes_to_transfer) break;

		if(!Stream.Modify.writeTo(to, buff, bytes_to_transfer)){
			return ERR(ERR.FAIL, "Failed to write to the file copy");
		}
	}

return OK;
}

std_Stream* moduleMethod(std_File, getStream){
	nonull(self) return nil;
return &priv.stream;
}
errvt moduleMethod(std_File, internalCopy, std_FS_Path newPath){

	var FSDevice = Filesys.getHandle(nil);

	if(!FSDevice)
		return ERR(ERR.INIT, "Failed to get FS device");
	
	errvt err = Stream.Modify.control(
		self, 
		std.FS.File.CtrlCmd.COPY, 
		generic newPath
	);

	if(!err) return OK;

	if(err != ERR.NOTIMPLEM) return ERR(ERR.FAIL, "Failed to copy file");

	var destHandle = Stream.open(
		FSDevice, 
		newPath, 
		Stream.Attrib.WRITE, 
		Filesys.File
	);

	if(!destHandle)
		return ERR(ERR.INIT, "Failed to get/create copy destination file");


	err = mod(manualCopy)(FSDevice, self, destHandle);

	if(err) return ERR(ERR.FAIL, "Failed to copy file");

	Stream.close(destHandle);

return OK;
}

errvt moduleMethod(std_File, cpy, std_FS_Path newPath){
	
	errvt err = OK;
	
    busy(priv.lock)
	err = mod(internalCopy)(self, newPath);
    meanwhile 
	return ERR(ERR.BUSY, "File is currently in use by another thread");

return err;
}
errvt moduleMethod(std_File, move, std_FS_Path newPath){

    busy(priv.lock){
	var FSDevice = Filesys.getHandle(nil);

	if(!FSDevice)
		return ERR(ERR.INIT, "Failed to get FS device");
	
	errvt err = Stream.Modify.control(
		self, 
		std.FS.File.CtrlCmd.MOVE, 
		generic newPath
	);

	if(!err) return OK;

	if(err != ERR.NOTIMPLEM) return ERR(ERR.FAIL, "Failed to move file");

	err = mod(internalCopy)(self, newPath);

	if(err) return ERR(ERR.FAIL, "Failed to copy file for move");

	err = Stream.delete(std.Stream.getHandle(&priv.stream));

	if(err) return ERR(ERR.FAIL, "Failed to delete file original file for move");

    } meanwhile 
	return ERR(ERR.BUSY, "File is currently in use by another thread");

return OK;
}

errvt moduleMethod(std_File, rename, std_FS_Path newName){

    busy(priv.lock){
	iferr(Stream.Modify.edit(
		std.Stream.getHandle(&priv.stream), 
		newName, 
		-1
	))
		return ERR(ERR.FAIL, "Failed to rename file");
    } meanwhile 
	return ERR(ERR.BUSY, "File is currently in use by another thread");

return OK;
}

errvt moduleMethod(std_File, update){

    busy(priv.lock){
	var stream = std.Stream.getHandle(&priv.stream);

	var info = Stream.Modify.info(stream);

	if(!info.valid)
		ERR(ERR.FAIL, "Failed to get updated file info");

	this.info.path 		= info.path;
	this.info.name 		= info.name;
	this.info.size 		= info.size;
	this.info.time_created 	= info.time_created;
	this.info.time_modified = info.time_modified;
    } meanwhile 
	return ERR(ERR.BUSY, "File is currently in use by another thread");

return OK;
}

errvt moduleMethod(std_File, delete, std_FS_Path atPath){
	if(self && atPath)
		return ERR(ERR.INVALID, "can only delete either from a path or an object");

    busy(priv.lock){
	if(self)
	    iferr(Stream.delete(std.Stream.getHandle(&priv.stream)))
		return ERR(ERR.FAIL, "Failed to delete file");
	else {
	    var FSDevice = Filesys.getHandle(nil);
	    if(!FSDevice)
		return ERR(ERR.FAIL, "Failed to get handle to filesystem");

	    var stream   = Stream.fetch(FSDevice, atPath, 0);

	    if(!stream)
	  	return ERR(ERR.FAIL, "Failed to get handle to file");

	    iferr(Stream.delete(stream))
		return ERR(ERR.FAIL, "failed to delete file");
	}
    } meanwhile 
	return ERR(ERR.BUSY, "File is currently in use by another thread");
	    
return OK;
}

errvt moduleMethod(std_File, createAt, std_FS_Path atPath){
	nonull(atPath) return err;

	var FSDevice = Filesys.getHandle(nil);
	if(!FSDevice)
		return ERR(ERR.FAIL, "Failed to get handle to filesystem");

	var handle = Stream.open(FSDevice, atPath, 0, Filesys.File);
	if(!handle)
	 	return ERR(ERR.FAIL, "Failed to create file");

	if(self && priv.stream.__type == nil){
		create_with(std_File, fromHandle, self, handle);

	} else iferr(Stream.close(handle))
		return ERR(ERR.FAIL, "Failed to properly close new file handle");
	
return OK;
}
errvt moduleMethod(std_File, writeData, void* data, len_t len){
	nonull(data, self) return err;

	if(!len) return OK;

    busy(priv.lock){
	var stream = std.Stream.getHandle(&priv.stream);

	if(!stream) 
		return ERR(ERR.FAIL, "Failed to get file stream handle");

	if(!Stream.Modify.writeTo(stream, data, len))
		return ERR(ERR.FAIL, "Failed to write to stream");
    } meanwhile
	return ERR(ERR.BUSY, "File is currently in use by another thread");

return OK;
}


errvt moduleMethod(std_File, readData, void* data, len_t len){
	nonull(data, self) return err;

	if(!len) return OK;

    busy(priv.lock){
	var stream = std.Stream.getHandle(&priv.stream);

	if(!stream) 
		return ERR(ERR.FAIL, "Failed to get file stream handle");

	if(!Stream.Modify.writeTo(stream, data, len))
		return ERR(ERR.FAIL, "Failed to write to stream");
    } meanwhile
	return ERR(ERR.BUSY, "File is currently in use by another thread");

return OK;
}

WRITE(std_File){
    nonull(self) return 0;

    len_t bytesWritten = 0;

    busy(priv.lock){
	bytesWritten = std.Types.data.writeTo(V(&priv.stream), data, size);
    } meanwhile {
	ERR(ERR.BUSY, "File is currently in use by another thread");
	return 0;
    }

return bytesWritten;
}
READ(std_File){
    nonull(self) return 0;

    len_t bytesWritten = 0;

    busy(priv.lock){
	bytesWritten = std.Types.data.readFrom(V(&priv.stream), data, size);
    } meanwhile {
	ERR(ERR.BUSY, "File is currently in use by another thread");
	return 0;
    }

return bytesWritten;
}

SIZE(std_File){
    nonull(self) return 0;

    len_t result = 0;

    busy(priv.lock){
	iferr(std.FS.File.update(self))
		ERR(ERR.FAIL, "Failed to update file info");

	result = this.info.size;
    } meanwhile {
	ERR(ERR.BUSY, "File is currently in use by another thread");
	return 0;
    }

return result;
}

HASH(std_File){
	return hash_bytes(
		generic this.info.path, 
		strsize(this.info.path, sizeof(std_FS_Path))
	);
}

SET(std_File){
	nonull(self, value) return err;

return mod(rename)(self, value);
}

DESTROY(std_File){
	var handle = std.Stream.getHandle(&priv.stream);

	pop(&priv.stream);

	if(!Stream.close(handle))
	   ERR(ERR.FAIL, "Failed to close stream handle");

return OK;
}

construct(std_File,
FMT(),
DEF(
.flags = std.FS.File.Flags.READ  | 
	 std.FS.File.Flags.WRITE |
	 std.FS.File.Flags.CREATE
),
){
	streamHandle handle = nil;
	var FSDevice = Filesys.getHandle(nil);

	if(!FSDevice){
		ERR(ERR.INIT, "Failed to get FS device");
	  	return nil;
	}

	word flags = 
	    (getbitflag(arg.flags, std.FS.File.Flags.WRITE)   ? Stream.Attrib.WRITE : 0) |
	    (getbitflag(arg.flags, std.FS.File.Flags.READ)    ? Stream.Attrib.READ  : 0) |
	    (getbitflag(arg.flags, std.FS.File.Flags.APPEND)  ? std.FS.File.Flags.APPEND  : 0)
	;


	if(getbitflag(arg.flags, std.FS.File.Flags.CREATE))
		handle = Stream.open(FSDevice, arg.path, flags, Filesys.File);
	else
	  	handle = Stream.fetch(FSDevice, arg.path, flags);
		
	if(!handle){
		ERR(ERR.INIT, "Failed to get/open file handle");
	  	return nil;
	}

	var info = Stream.Modify.info(handle);

	if(!info.valid){
		ERR(ERR.INIT, "Failed to get info on file");
	  	if(!Stream.close(handle))
		    ERR(ERR.FAIL, "Failed to close stream handle");
	  	
	  	return nil;
	}

	this.info.path 		= info.path;
	this.info.name 		= info.name;
	this.info.size 		= info.size;
	this.info.time_created 	= info.time_created;
	this.info.time_modified = info.time_modified;

	if(!create(std_Stream, &priv.stream, std.Stream.Preset.fromHandle(handle))){
		ERR(ERR.INIT, "Failed to create stream object from handle");
	  	if(!Stream.close(handle))
		    ERR(ERR.FAIL, "Failed to close stream handle");
	  	return nil;
	}

return self;
}

construct_with(std_File, fromHandle, DEF()){
	nonull(arg.handle, self) return nil;
	
	var info = Stream.Modify.info(arg.handle);

	if(!info.valid){
		ERR(ERR.INIT, "Failed to get info on file");
	  	return nil;
	}

	if(info.type != Filesys.File.type){
		ERR(ERR.INVALID, "stream handle is not a file");
		return nil;
	}

	this.info.path 		= info.path;
	this.info.name 		= info.name;
	this.info.size 		= info.size;
	this.info.time_created 	= info.time_created;
	this.info.time_modified = info.time_modified;

	if(!create(std_Stream, &priv.stream, std.Stream.Preset.fromHandle(arg.handle))){
		ERR(ERR.INIT, "Failed to create stream object from handle");
	  	return nil;
	}

return self;
}

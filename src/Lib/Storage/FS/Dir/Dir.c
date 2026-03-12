#include "../FS.h"
#define module std, FS, Dir

errvt moduleFn(manualCopy)(devHandle io_dev, streamHandle from, streamHandle to){ 

	i64 entries_read = 0;
	streamInfo temp_entries_store[10] = {};
	var nested_dirs   = pushArrayStack(streamHandle, 10);
	var entries_list  = pushArrayList(streamInfo, 10);

	std_FS_Path path_buff = {};
	Path.start(&path_buff);
		
	//Push the initial directories 
	write(nested_dirs, &from, &to);

	do{
		streamHandle curr_dir = nil, curr_new_dir = nil;

		//Pop the current directory to copy from and to
		read(nested_dirs, &curr_dir, &curr_new_dir);

		//Setup path buff with the new directory path
		streamInfo new_dir_info = Stream.Modify.info(curr_new_dir);
		set_use(std_FS_Path_Type, path_buff, new_dir_info.path);
	 
		//Read all entries from the previous directory
		while((entries_read = Stream.Modify.readFrom(curr_dir, temp_entries_store, 10)) != 0)
			std.Array.Write(entries_list, temp_entries_store, entries_read);
		
		//Iterate through all of the entries
		foreach(entries_list, streamInfo, entry){

			//Add file/dir name to new directory path
			len_t entry_name_len = strsize(entry->name, sizeof(std_FS_Path));
			Path.rename(entry->name);

			if(entry->type == Filesys.Dir.type){
				streamHandle
				sub_dir     = Stream.fetch(io_dev, entry->path, Stream.Attrib.READ),
				sub_dir_new = Stream.open(io_dev, path_buff, Stream.Attrib.WRITE, Filesys.Dir);

				write(nested_dirs, &sub_dir, &sub_dir_new);
			}
			elif(entry->type == Filesys.Dir.type){
				streamHandle
				sub_file     = Stream.fetch(io_dev, entry->path, Stream.Attrib.READ),
				sub_file_new = Stream.open(io_dev, path_buff, Stream.Attrib.WRITE, Filesys.File);

				std_FS_File_manualCopy(io_dev, sub_file, sub_file_new);

				Stream.close(curr_dir);
				Stream.close(curr_new_dir);
			}
			else{ ERR(ERR.INVALID, "Invalid stream type found while copying directory, skipping..."); }
		}

		if(curr_dir != from)   Stream.close(curr_dir);
		if(curr_new_dir != to) Stream.close(curr_new_dir);
	}
	while(len(nested_dirs) != 0);

	Path.end();

	pop(nested_dirs, entries_list);

return OK;
}

std_Stream* moduleMethod(std_Dir, getStream){
	nonull(self) return nil;
return &priv.stream;
}

errvt moduleMethod(std_Dir, internalCopy, std_FS_Path newPath){

    busy(priv.lock){
	var FSDevice = Filesys.getHandle(nil);

	if(!FSDevice)
		return ERR(ERR.INIT, "Failed to get FS device");
	
	errvt err = Stream.Modify.control(
		self, 
		std.FS.Dir.CtrlCmd.COPY, 
		generic newPath
	);

	if(!err) return OK;

	if(err != ERR.NOTIMPLEM) return ERR(ERR.FAIL, "Failed to copy directory");

	var destHandle = Stream.open(
		FSDevice, 
		newPath, 
		Stream.Attrib.WRITE, 
		Filesys.Dir
	);

	if(!destHandle)
		return ERR(ERR.INIT, "Failed to get/create copy destination directory");


	err = mod(manualCopy)(FSDevice, self, destHandle);

	if(err) return ERR(ERR.FAIL, "Failed to copy directory");

	Stream.close(destHandle);

    } meanwhile 
	return ERR(ERR.BUSY, "Directory is currently in use by another thread");

return OK;
}

errvt moduleMethod(std_Dir, cpy, std_FS_Path newPath){

	errvt err = OK;
	
    busy(priv.lock)
	err = mod(internalCopy)(self, newPath);
    meanwhile 
	return ERR(ERR.BUSY, "Directory is currently in use by another thread");

return err;
}

errvt moduleMethod(std_Dir, move, std_FS_Path newPath){

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

	if(err != ERR.NOTIMPLEM) return ERR(ERR.FAIL, "Failed to move directory");

	err = mod(internalCopy)(self, newPath);

	if(err) return ERR(ERR.FAIL, "Failed to copy directory for move");

	err = Stream.delete(std.Stream.getHandle(&priv.stream));

	if(err) return ERR(ERR.FAIL, "Failed to delete directory original directory for move");

    } meanwhile 
	return ERR(ERR.BUSY, "Directory is currently in use by another thread");

return OK;
}

errvt moduleMethod(std_Dir, rename, std_FS_Path newName){

    busy(priv.lock){
	iferr(Stream.Modify.edit(
		std.Stream.getHandle(&priv.stream), 
		newName, 
		-1
	))
		return ERR(ERR.FAIL, "Failed to rename directory");
    } meanwhile 
	return ERR(ERR.BUSY, "Directory is currently in use by another thread");

return OK;
}

errvt moduleMethod(std_Dir, update){

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

errvt moduleMethod(std_Dir, delete, std_FS_Path atPath){

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

errvt moduleMethod(std_Dir, createAt, std_FS_Path atPath){
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

errvt moduleMethod(std_Dir, addEntries, Array(std_FS_Entry*) entries){
	nonull(self) return err;

	if(!len(&entries)) return OK;

    busy(priv.lock){
	var stream = std.Stream.getHandle(&priv.stream);

	if(!stream) 
		return ERR(ERR.FAIL, "Failed to get directory stream handle");

	var tempBuff = pushArrayBuffer(streamInfo, len(&entries));

	foreach(&entries, std_FS_Entry*, entrypntr){
		var entry 	= *entrypntr;
		var entInfo 	= entry->info;

		streamInfo info = {};

		info.type = 
			(TFrom(entry).id == T(std_File).id) ? Filesys.File.type :
			(TFrom(entry).id == T(std_Dir).id)  ? Filesys.Dir.type :
			nil;

		info.name = entInfo.name;

		if(!info.type || !info.name)
			continue;

		write(tempBuff, &info);
	}

	if(!Stream.Modify.writeTo(stream, tempBuff->data, tempBuff->items)){
		pop(tempBuff);
		return ERR(ERR.FAIL, "Failed to add entries to directory stream");
	}

	pop(tempBuff);

    } meanwhile
	return ERR(ERR.BUSY, "Directory is currently in use by another thread");

return OK;
}

errvt moduleMethod(std_Dir, readEntries, ArrayBuffer(std_FS_Entry*) buffer){
	nonull(buffer, self) return err;

	if(!len(buffer)) return OK;

    busy(priv.lock){
	var stream = std.Stream.getHandle(&priv.stream);

	if(!stream) 
		return ERR(ERR.FAIL, "Failed to get directory stream handle");

	if(!Stream.Modify.writeTo(stream, buffer->data, len(buffer)))
		return ERR(ERR.FAIL, "Failed to read entries from directory stream");

    } meanwhile
	return ERR(ERR.BUSY, "Directory is currently in use by another thread");

return OK;
}


construct(std_Dir,
FMT(),
DEF(),

){
	streamHandle handle = nil;
	var FSDevice = Filesys.getHandle(nil);

	if(!FSDevice){
		ERR(ERR.INIT, "Failed to get FS device");
	  	return nil;
	}

	word flags = 
	    (getbitflag(arg.flags, std.FS.Dir.Flags.WRITE) ? Stream.Attrib.WRITE : 0) |
	    (getbitflag(arg.flags, std.FS.Dir.Flags.READ)  ? Stream.Attrib.READ  : 0)
	;


	if(getbitflag(arg.flags, std.FS.Dir.Flags.CREATE))
		handle = Stream.open(FSDevice, arg.path, flags, Filesys.File);
	else
	  	handle = Stream.fetch(FSDevice, arg.path, flags);
		
	if(!handle){
		ERR(ERR.INIT, "Failed to get/open directory handle");
	  	return nil;
	}

	var info = Stream.Modify.info(handle);

	if(!info.valid){
		ERR(ERR.INIT, "Failed to get info on directory");
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

construct_with(std_Dir, fromHandle, DEF()){
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

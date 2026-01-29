#include <XC.pkg.c>

#define module std, FS

alias(core.Device, 	    Dev)
alias(core.Device.Stream,   Stream)
alias(core.Device.Register, Reg)

std_Stream* moduleFn(temp)(int flags, std_FSEntry* ent_buff){
	devHandle io_dev = Dev.stdHandle(Dev.ID.IO);

	DO_LATER("Parse the flags and fix the parameters passed to Dev.Stream.open()");

	streamHandle strm = Dev.Stream.open(io_dev, nil, flags, nil);

return new(std_Stream, std.Stream.Preset.fromHandle(strm));


}

errvt moduleFn(update)(std_FSEntry* ent){
	devHandle io_dev = Dev.stdHandle(Dev.ID.IO);
	streamHandle strm = Stream.fetch(io_dev, ent->path, 0);
	
	if(strm == nil) 
		return ERR(ERR.FAIL, "Failed to update file entry info");
	
	streamInfo info = core.Device.Stream.Modify.info(strm);

	*ent = (std_FSEntry){
	    .isdir   	   = info.type == &core.Device.Stream.Type.DIR,
	    .name 	   = generic info.name,
	    .size 	   = info.size,
	    .time_created  = info.time_created,
	    .time_modified = info.time_modified,
	    .stream 	   = new(std_Stream, std.Stream.Preset.fromHandle(strm)),
	    .__type 	   = std_FSEntry_Type,
	};

}

errvt moduleFn(move)(std_FSPath from, std_FSPath to){
	nonull(from, to) return err;

	iferr(std.FS.cpy(from, to)) return err;

	iferr(std.FS.delete(from)) return ERR(ERR.FAIL, 
	     "Failed to delete the old entry, it has only been copied");
	
return OK;
}

errvt moduleFn(cpyFile)(devHandle io_dev, streamHandle from, streamHandle to){ 
	u64 bytes_to_transfer = 0, cursor_offset = 0;

	for(;;){
		u8 buff[1028];

		bytes_to_transfer = Stream.Modify.readFrom(from, buff, 1028);

		if(!bytes_to_transfer) break;

		if(!Stream.Modify.writeTo(to, buff, bytes_to_transfer)){
			return ERR(ERR.FAIL, "Failed to write to the file copy");
		}
	}

	Stream.close(from);
	Stream.close(to);

return OK;
}

errvt moduleFn(cpyDir)(devHandle io_dev, streamHandle from, streamHandle to){ 

	i64 entries_read = 0;
	streamInfo temp_entries_store[10] = {0};
	var nested_dirs   = pushArrayStack(streamHandle, 10);
	var entries_list  = pushArrayList(streamInfo, 10);
	var new_path_buff = pushArrayStack(c8, sizeof(std_FSPath));
	
	write(nested_dirs, &from, &to);
	do{
		streamHandle curr_dir = nil, curr_new_dir = nil;

		read(nested_dirs, &curr_dir, &curr_new_dir);

		streamInfo new_dir_info = Stream.Modify.info(curr_new_dir);

		std.Array.Write(new_path_buff, new_dir_info.path, strsize(new_dir_info.path, sizeof(std_FSPath)));
		std.Array.Write(new_path_buff, "/", 1);

		while((entries_read = Stream.Modify.readFrom(curr_dir, temp_entries_store, 10)) != 0)
			std.Array.Write(entries_list, temp_entries_store, entries_read);
		
		foreach(entries_list, streamInfo, entry){
			len_t entry_name_len = strsize(entry->name, sizeof(std_FSPath));
			std.Array.Write(new_path_buff, entry->name, entry_name_len);

			var new_path = std.Array.ToPointer(new_path_buff);

			if(entry->type == &Stream.Type.DIR){
				streamHandle
				sub_dir     = Stream.fetch(io_dev, entry->path, Stream.Attrib.READ),
				sub_dir_new = Stream.open(io_dev, new_path, Stream.Attrib.WRITE, &Stream.Type.DIR);

				write(nested_dirs, &sub_dir, &sub_dir_new);
			}
			elif(entry->type == &Stream.Type.FILE){
				streamHandle
				sub_file     = Stream.fetch(io_dev, entry->path, Stream.Attrib.READ),
				sub_file_new = Stream.open(io_dev, new_path, Stream.Attrib.WRITE, &Stream.Type.FILE);

				mod(cpyFile)(io_dev, sub_file, sub_file_new);

				Stream.close(curr_dir);
				Stream.close(curr_new_dir);
			}
			else{ ERR(ERR.INVALID, "Invalid stream type found while copying directory, skipping..."); }
			std.Array.Read(new_path, nil, entry_name_len);
		}

		std.Array.Clear(new_path_buff);
		Stream.close(curr_dir);
		Stream.close(curr_new_dir);
	}
	while(len(nested_dirs) != 0);

	pop(nested_dirs, entries_list, new_path_buff);

return OK;
}


errvt moduleFn(cpy)(std_FSPath from, std_FSPath to){
	nonull(from, to) return err;

	var io_dev = Dev.stdHandle(Dev.ID.IO);
	
	var from_strm = Stream.fetch(io_dev, from, Stream.Attrib.READ);
	if(!from_strm)
		return ERR(ERR.FAIL, "Failed to fetch stream handle to copy from");

	var from_info = Stream.Modify.info(from_strm);
	if(!from_info.valid)
		return ERR(ERR.FAIL, "Failed to get info on stream handle to copy from");

	var to_strm = Stream.open(io_dev, to, Stream.Attrib.WRITE, from_info.type);
	if(!to_strm)
		return ERR(ERR.FAIL, "Failed to fetch or create stream handle to copy into");


	if(from_info.type == &Stream.Type.FILE) 
		return mod(cpyFile)(io_dev, from_strm, to_strm);

	elif (from_info.type == &Stream.Type.DIR)
		return mod(cpyDir)(io_dev, from_strm, to_strm);

	else {
		errvt err = ERR(ERR.INVALID, "Invalid stream type");
		Stream.close(from_strm);	
		Stream.close(to_strm);	
		return err;
	}
}


std_Stream* moduleFn(open)(std_FSPath path, int flags){
	devHandle io_dev = Dev.stdHandle(Dev.ID.IO);
	
	DO_LATER("Parse the flags and fix the parameters passed to Dev.Stream.open()");

	streamHandle strm = Dev.Stream.open(io_dev, path, flags, nil);

return new(std_Stream, std.Stream.Preset.fromHandle(strm));
}


errvt moduleFn(delete)(std_FSPath path){
	devHandle io_dev = Dev.stdHandle(Dev.ID.IO);
	
	streamHandle strm = Stream.fetch(io_dev, path, 0);

	core.Device.Stream.delete(strm);
return OK;
}


errvt moduleFn(chdir)(std_FSPath path){
	devHandle io_dev = Dev.stdHandle(Dev.ID.IO);
	
	registerHandle curr_dir = Reg.stdHandle(Reg.ID.WorkDir);

	Reg.Modify.writeTo(curr_dir, path, strnlen((char*)path, sizeof(std_FSPath)));
return OK;
}
std_Stream* moduleFn(search)(std_FSPath path, std_FSEntry* ent){
	devHandle io_dev = Dev.stdHandle(core.Device.ID.IO);
	
	streamHandle strm = Stream.fetch(io_dev, path, 0);
	
	if(strm == nil) {
		return nil;
	}

	streamInfo info = core.Device.Stream.Modify.info(strm);

	*ent = (std_FSEntry){
	    .isdir   	   = info.type == &core.Device.Stream.Type.DIR,
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

errvt moduleFn(rename)(std_FSPath path, std_FSPath new_name){
	devHandle io_dev = core.Device.stdHandle(core.Device.ID.IO);
	
	streamHandle strm = core.Device.Stream.fetch(io_dev, path, 0);

	core.Device.Stream.edit(strm, new_name, 0);
	
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
  	generic data, size
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
	.Print = nil, // planned for next release
	.Scan  = nil, // planned for next release
	
){ 
	if(std.FS.open(arg.path, args->flags, self) == nil){
		ERR(ERR.FAIL, "failed to open FS entry");
	  	return nil;
	}

return self;
}
 


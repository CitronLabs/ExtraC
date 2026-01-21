#include "IO.h"


IO_Stream* moduleFn(IO_Open_File)(const char* path, word attributes, bool create){
	LPOFSTRUCT fileInfo  =  0;
	int	   fileFlags =  0;

	if(attributes & core.Device.Stream.Attrib.READ)
		fileFlags = OF_READ;

	if(attributes & core.Device.Stream.Attrib.WRITE){
	    if(!fileFlags)
		fileFlags = OF_WRITE;
	    else
		fileFlags = OF_READWRITE;
	}

	if(fileFlags == 0){
		ERR(ERR.INVALID, "IO stream is neither being opened for write or read");
		return nil;
	}

	HFILE fileHandle = OpenFile(path, fileInfo, fileFlags | OF_CREATE);

	if(fileHandle == HFILE_ERROR){
		strc16 errmsg = WinUtils.Error.getMsg(GetLastError());

		ERR(ERR.FAIL, "Failed to open IO stream");
		printlnErr("Reason: ", $(errmsg));

		WinUtils.Error.freeMsg(errmsg);
		return nil;
	}

	len_t pathSize    = strsize(path, PATH_MAX);
	IO_Stream* result = malloc(sizeof(IO_Stream) + pathSize);

	result->handle = fileHandle;
	result->path   = memcpy(
				pntr_shiftcpy(result, sizeof(IO_Stream)), 
				generic path, 
				pathSize
			);
return result;
}

errvt moduleFn(Sys_Locale_onInit)(devID device, rsrcID resource){
	Manager* devManager = WinRTDev.getManager();

	IO_Stream_Init* initSettings = Dev.Resource.fetch(
						devManager,
						device,
						resource
					).object;

	var stream = mod(IO_Open_File)(
			initSettings->path, 
			initSettings->attributes, 
			initSettings->create
		     );

	if(stream == nil)
		return ERR(ERR.FAIL, "Failed to create IO stream");
	

}

errvt moduleFn(Sys_Locale_onExit)(devID device, rsrcID resource){
}

errvt fn(watch)(streamHandle handle);
errvt fn(isModified)(streamHandle handle);

len_t fn(shift)(streamHandle handle, word offset, len_t from);
len_t fn(readFrom)(streamHandle handle, void* buffer, len_t size);
len_t fn(writeTo)(streamHandle handle, const void* buffer, len_t size);
streamInfo fn(info)(streamHandle handle);

errvt fn(control)(streamHandle handle, word command, void* args); // Generic IOCTL/FCNTL abstraction
errvt fn(flush)(streamHandle handle); // Forces pending writes to the underlying medium
errvt fn(sync)(streamHandle handle); // Ensures data and metadata are written (fsync)




Resource(IO, File, STREAM,


)

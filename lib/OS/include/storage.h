#pragma once
#include "./extern.h"
#include "./user.h"
#include <stddef.h>

#define FS_DIR true
#define FS_FILE false

typedef void* storageHandle;


type(storageDevice,
	inst(String) name;
	inst(String) manufacturer;
	inst(String) model;

	void* uniqueID;
	len_t  // The size in bytes of a single unit 
		// a.k.a KiB == 1,000, MiB = 100,000, etc
		measure,  
		exponant,	// storage_size = (measure * value)^exponant
		value;
)


Interface(storage,
    namespace(device,
	
    )
    namespace(fs,
	namespace(ext,
		const bool implemented;
		errvt vmethod(readLink,     fsPath path, fsPath result)
		errvt vmethod(makeLink,     fsPath path, fsPath result)
		errvt vmethod(changePerms,  fsPath path, userPermissions perms);
		errvt vmethod(changeOwner,  fsPath path, userHandle user);
	);	
    )

	i64 	 vmethod(write,        storageHandle handle, pntr data, len_t size);
	i64 	 vmethod(read,         storageHandle handle, pntr data, len_t size);
	errvt  	 vmethod(close,        storageHandle handle);
	errvt  	 vmethod(handleEvents, storageHandle handle, Queue(OSEvent) evntQueue);
)


enum(FileSysEvent_Type,
    FileSysEvent_Write,
    FileSysEvent_Read,
    FileSysEvent_Delete
)
type(FileSysEvent,
    storageHandle handle;
    FileSysEvent_Type type;
)

#include "../Device.h"



errvt moduleFn(Resource_StdErr_open)(streamHandle handle, bool create);
errvt moduleFn(Resource_StdErr_close)(streamHandle handle);	
errvt moduleFn(Resource_StdErr_delete)(streamHandle handle);	
errvt moduleFn(Resource_StdErr_edit)(streamHandle handle, const char* name, word attributes);
errvt moduleFn(Resource_StdErr_watch)(streamHandle handle);
len_t moduleFn(Resource_StdErr_isModified)(streamHandle handle);

len_t moduleFn(Resource_StdErr_shift)(streamHandle handle, word offset, len_t from);
len_t moduleFn(Resource_StdErr_readFrom)(streamHandle handle, void* buffer, len_t size);
len_t moduleFn(Resource_StdErr_writeTo)(streamHandle handle, const void* buffer, len_t size);
streamInfo moduleFn(Resource_StdErr_info)(streamHandle handle);

errvt moduleFn(Resource_StdErr_control)(streamHandle handle, word command, void* args); // Generic IOCTL/FCNTL abstraction
errvt moduleFn(Resource_StdErr_flush)(streamHandle handle); // Forces pending writes to the underlying medium
errvt moduleFn(Resource_StdErr_sync)(streamHandle handle); // Ensures data and metadata are written (fsync)

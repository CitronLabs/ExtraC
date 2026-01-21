
errvt moduleFn(Resource_WorkDir_open)(registerHandle handle, bool create);	
errvt moduleFn(Resource_WorkDir_close)(registerHandle handle);	
errvt moduleFn(Resource_WorkDir_delete)(registerHandle handle);	
errvt moduleFn(Resource_WorkDir_edit)(registerHandle handle, const char* name, word attributes);
errvt moduleFn(Resource_WorkDir_watch)(registerHandle handle);
errvt moduleFn(Resource_WorkDir_isModified)(registerHandle handle);

len_t moduleFn(Resource_WorkDir_readFrom)(registerHandle handle, const void* buffer, len_t size);
len_t moduleFn(Resource_WorkDir_writeTo) (registerHandle handle, const void* buffer, len_t size);
len_t moduleFn(Resource_WorkDir_setTo)   (registerHandle handle, const void* buffer, len_t size);
void* moduleFn(Resource_WorkDir_access)  (registerHandle handle);

registerInfo moduleFn(Resource_WorkDir_info)(registerHandle handle);

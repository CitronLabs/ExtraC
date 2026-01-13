
errvt 	   fn(edit)	    (word resourceType, pntr handle, const char* name, word attributes);	
errvt 	   fn(close)	    (word resourceType, pntr handle);
pntr 	   fn(fetch)	    (word resourceType, const char* name, word attributes, void* type);
pntr       fn(open) 	    (word resourceType, const char* name, word attributes, void* type);
errvt 	   fn(delete)	    (word resourceType, pntr handle);
deviceInfo fn(info) 	    (devHandle handle); 

#pragma once
#define __XC_OS_SYSTEM_GRAPHICS__
#include "../pkg.h"


#ifndef __XC_OS_SYSTEM_GRAPHICS_RENDER__
#include "../types.h"
#include "types.h"
#define package os_Sys_Graphics

Interface(Render,
	values(MemoryType, u32,
		DEVICE_LOCAL,
		HOST_VISIBLE,
		HOST_COHERENT,
		CACHED,
		PROTECTED
	)

	values(Features, u32,
		PIXEL_BLIT,
	  	GEOMETRY,
	  	TEXT,
	  	LAYOUT,
	  	COMPUTE,
	  	RAY_TRACING,
	  	MESH_SHADING,
	  	VIDEO_DECODE,
	  	HDR10,
	  	ALPHA_BLEND,
	  	SCALING_HARDWARE
	)


	Array(GraphicsDevice) 	fn(enumDevices)();
	
	/* --- Lifecycle --- */
	errvt fn(setup)(pkg(Handle) handle, pkg(Handle) display);
	errvt fn(destroy)(pkg(Handle) handle);

  submodule(Resource,
	values(Type, u32,
		LINEAR,
		IMAGE,
		TREE,
		STATE,
		SIGNAL
	)
	values(Flag, u32,
		READ,
		WRITE,
		TRANSIANT,
		SOURCE,
		DEST,
		RENDER_TARGET,
		PERSISTENT
	)
    	pkg(Render_ResourceID) 	fn(init)(pkg(Handle) handle,  u32 type_id, len_t size, u32 flags);
    	errvt 			fn(destroy)(pkg(Handle) handle, pkg(Render_ResourceID) res);
    	
    	void* 			fn(map)(pkg(Handle) handle, pkg(Render_ResourceID) res);
    	errvt 			fn(unmap)(pkg(Handle) handle, pkg(Render_ResourceID) res);
    	
    	errvt 			fn(flush)(pkg(Handle) handle, pkg(Render_ResourceID) res, len_t offset, len_t size);

    	errvt 			fn(bind)(pkg(Handle) handle, pkg(Render_ResourceID) res, u32 slot);
  )
  submodule(Execution,
	values(Model, u32,
		IMMEDIATE,
	    	DEFERRED,
		CONSUMER,
		ASYNC_COMPUTE,
		REMOTE,
		VIRTUALIZED
	)
  submodule(Command,
	values(Display, u32,
		CLEAR_COLOR,
	    	CLEAR_DEPTH,
	    	SET_CLIPPING
	)
	values(Resource, u32,
	    	COPY_RESOURCE,
	    	UPDATE_RESOURCE,
	    	PUSH_CONSTANTS
	)
	values(Execute, u32, 
	    	DRAW_INSTANCED,
	    	DISPATCH_COMPUTE,
		UPDATE_TREE
	)
  )
    	// Executes an operation (DRAW, BLIT, CLEAR, COMPUTE_DISPATCH)
    	errvt fn(execute)(pkg(Handle) handle, u32 command_id, void* payload);

    	// Swaps buffers. If fence is non-NULL, it signals when the back-buffer is safe to write again.
    	errvt fn(swapBuffers)(pkg(Handle) handle, pkg(Render_ResourceID) back_buffer);
  )
)


#undef package
#endif

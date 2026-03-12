#pragma once
#define __XC_MEDIA_GRAPHICS__
#include "../pkg.h"
#include "../Window/pkg.h"
#include "../Screen/pkg.h"
#include "../Terminal/pkg.h"
#include "../Resource/pkg.h"


#ifndef __XC_MEDIA_GRAPHICS_RENDERER__
#include "types.h"

#define package std

Class(Renderer,
INIT(strc8 name; std_Window* window),
FMT(),
	std_Renderer_Info info;
	std_Stream stream;
){

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

    	errvt 			method(Renderer, map,    std_Graphics_Resource* res);
    	errvt 			method(Renderer, unmap,  std_Graphics_Resource* res);

    	errvt 			method(Renderer, bind,   std_Graphics_Resource* res, i32 slot);
    	errvt 			method(Renderer, unbind, std_Graphics_Resource* res);
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
    	errvt method(Renderer, execute, u32 command_id, void* payload);

    	// Swaps buffers. If fence is non-NULL, it signals when the back-buffer is safe to write again.
    	errvt method(Renderer, swapBuffers, std_Graphics_Resource* back_buffer);
  )

};

constructor(Renderer, Screen,   strc8 name; std_Screen* screen);
constructor(Renderer, Terminal, strc8 name; std_Terminal* terminal);

#undef package
#endif

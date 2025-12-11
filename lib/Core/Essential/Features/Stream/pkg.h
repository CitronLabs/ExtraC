#pragma once

#define __XC_FEATURE__

#include "../pkg.h"

#include "../Type/pkg.h"
#include "../Arrays/pkg.h"

//=======EXPORTS========
#ifndef __XC_STREAM__


#include "types.h"
#include "utils.h"
#include "FS/pkg.h"

#define package std

Class(Stream, 
INIT(std_Stream_Options ops), 
FMT(),
private(
	len_t 		frameSize;

	struct {
   	  u16 
	    readOnly   	: 1,
	    writeOnly  	: 1,
	    streamType  : 2;
      	} flags;

	union {
	    streamHandle handle;

	    struct {
	        std_Array_List* data;
	        len_t pos;
      	    } mem;

	    struct {
	        len_t pos, size;
	        void* data;
	    } buff;
	} stream;
	pntr pointer;
)
){
      	submodule(Preset,
		const std_Stream_Options 
			fn(staticBuffer)(void* start, len_t len),
			fn(fromHandle)(streamHandle handle)
		;
	)

	std_Stream*    
		fn(stdOut)(),
		fn(stdIn)(),
		fn(stdErr)()
	;

	streamHandle method(Stream, getHandle);

	errvt method(Stream, Flush);	
	void* method(Stream, ToPointer);
	void* method(Stream, GetPointer);
	len_t method(Stream, GetCursorPos);

      	const struct Stream_Proc {
		struct Stream_Proc 
	        fn(start)(std_Stream*),
		fn(cont)(),
		fn(doEncode)(std_StreamEncoder, void*),
		fn(doDecode)(std_StreamDecoder),
		fn(each)(len_t),
		fn(advance)(len_t),
		fn(rewind)(len_t),
		fn(readData)(void*, len_t),
		fn(writeData)(void*, len_t),
		fn(setProc)(const struct Stream_Proc**);

	    std_Stream    
		* fn(current)();

		
	    noFail fn(end)(), fn(fail)(), fn(doRun)(), fn(pause)();
	
	    pntr fn(next)(pntr*);
	
	}Process;
};

#undef package

#endif






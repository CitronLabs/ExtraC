#pragma once
#define __XC_STREAM__
#include "pkg.h"

#define package std

// if setMemoryAddr is set to a memory address then
// the resulting stream will write input to the memory address 
type(Stream_Options,
	struct{ void* data; len_t len; }init;
	void* setMemoryAddr;
	len_t   
	   setMemorySize,
	   frameSize
	;
       	struct {
   		u16 readOnly   	: 1;
		u16 writeOnly  	: 1;
		u16 invalid	: 1;
       	} flags;

	streamHandle handle;
)

type_fn(errvt, StreamDecoder, std_Stream*, void*);
type_fn(errvt, StreamEncoder, std_Stream*, void*);

#define StreamEncoder(name) errvt name##_StreamEncoder(std_Stream* stream, void* data)
#define StreamDecoder(name) errvt name##_StreamDecoder(std_Stream* stream, void* data)

#undef package

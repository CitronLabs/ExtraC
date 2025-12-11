#pragma once
#define __XC_ENV__
#include "../pkg.h"

#ifndef __XC_ENV_LIB__
	#include "utils.h"
	#include "types.h"

Module(XC){
	values(Version, word,
		MAJOR,
		MINOR,
		PATCH
	);
  submodule(Dev,
    submodule(Register,
	values(Attrib, word,
	    READ,
	    WRITE,
	    DIRECT_ACCESS
	)
	word fn(open)(devHandle dev, word attributes, len_t size, strc8 name);	
	word fn(find)(devHandle dev, strc8 name);	

	registerHandle fn(fetch)(devHandle dev, word id);

	errvt fn(drop)(registerHandle handle);
	errvt fn(close)(devHandle dev, word id);

	
	len_t fn(readFrom)(registerHandle handle, void* buffer, len_t size);
	len_t fn(writeTo)(registerHandle handle, const void* buffer, len_t size);

	void* fn(access)(registerHandle handle);

	registerInfo fn(info)(registerHandle handle);
	
	values(ID, word,
		Errno,
		Locale
	)
	registerHandle fn(stdHandle)(word id); 
    )
       
    submodule(Stream,
	values(Attrib, word,
	    READ,
	    WRITE,
	    DIR,
	    LINK
	)
	// open can be used to create a stream as well as grabbing the streamHandle
	streamHandle fn(open)(devHandle dev, const char* key, word attributes, streamSettings* settings);	

	// fetch can be used to check if a stream exists as well as grabbing the streamHandle
	streamHandle fn(fetch)(devHandle dev, const char* key, word attributes, streamSettings* settings); 	
	
	errvt fn(watch)(streamHandle handle);
	errvt fn(isModified)(streamHandle handle);
	errvt fn(modify)(streamHandle handle, const char* key, word attributes);	
	errvt fn(drop)(streamHandle handle);
	errvt fn(close)(streamHandle handle);
	
	values(ID, word,
		In,
		Out,
		Err
	)
	streamHandle fn(stdHandle)(word id); // For stdin/stdout/stderr

	len_t fn(shift)(streamHandle handle, word offset, len_t from);
	len_t fn(readFrom)(streamHandle handle, void* buffer, len_t size);
	len_t fn(writeTo)(streamHandle handle, const void* buffer, len_t size);
	streamInfo fn(info)(streamHandle handle);

 	errvt fn(control)(streamHandle handle, word command, void* args); // Generic IOCTL/FCNTL abstraction
        errvt fn(flush)(streamHandle handle); // Forces pending writes to the underlying medium
        errvt fn(sync)(streamHandle handle); // Ensures data and metadata are written (fsync)
    )
	values(Attrib, word,
		PUBLIC,
		PRIVATE,
		REGISTER_CREATE,
		STREAM_CREATE
	)

	devHandle fn(open)(const char* name, word attributes);
	devHandle fn(fetch)(const char* name);

	values(ID, word,
		Sys,
		Local,
		IO
	)
	devHandle  fn(stdHandle)(word id); 
	deviceInfo fn(info)(devHandle handle); 
	errvt 	   fn(close)(devHandle handle);
  )

  submodule(Mem,
	void* fn(alloc)(len_t num_pages);
	errvt fn(dealloc)(void* ptr, len_t num_pages);
	len_t fn(getPageSize)();
	
	values(Perms, word,
		READ,
		WRITE,
		EXEC
	)
	errvt fn(protect)(
		void* ptr, 
		len_t num_pages, 
		errvt fn(fault_callback)(void*, len_t), 
		word permissions
	);
  )
  submodule(Time,
	values(Source, word,
		REALTIME,
		MONOTONIC
	)
      	errvt fn(getNow)(word src, timeSpec* ts);
      
      	errvt fn(sleep)(const timeSpec* duration);
      
      	len_t fn(tickCount)(); 
      	len_t fn(tickFreq)(); 
  )
  
  submodule(Sys,
    submodule(Arch,
	void* fn(getInstructionPtr)();
	
	void  fn(jmp)(void*);

	void* fn(saveRegs)(sysRegisters*);

	void  fn(loadRegs)(sysRegisters*);

        void* fn(getStackPtr)();
        void  fn(setStackPtr)(void* ptr);
        void* fn(getFramePtr)();
        void  fn(setFramePtr)(void* ptr);

        // Add-with-Carry: *sum_out = op1 + op2 + carry_in
        word fn(addCarry)(word* sum_out, word operand1, word operand2, word carry_in);

        // Subtract-with-Borrow: *diff_out = op1 - op2 - borrow_in
        word fn(subCarry)(word* diff_out, word operand1, word operand2, word borrow_in);

        XC_Sys_Arch_Flags 
	     fn(getFlags)();

        void fn(setFlags)(XC_Sys_Arch_Flags flag_word);
    )
    submodule(Locale,
	values(Temp, word,
		Celsius,
		Fahrenheit
	)
	values(Measure, word,
		Metric,
		Imperial
	)
    )
      	errvt fn(execute)(const char* command);
      
	values(ExitCode, word,
		FAILURE,
	      	SUCCESS
	)
      	void fn(terminate)(word status, word flags); 

      	bool fn(saveState)(stateData* state);
      	errvt fn(loadState)(stateData state);

  )
  submodule(Vec,
    submodule(Mem,
    	void* fn(cpy)(void* dest, const void* src, len_t size);
    
    	word fn(cmp)(const void* buffer1, const void* buffer2, len_t size);
    
    	void* fn(setVal)(void* dest, word value, len_t size);

    	// 'pattern_size' MUST be a power of 2 (e.g., 4, 8, 16) for optimal vectorization.
    	void* fn(fillPattern)(void* dest, const void* pattern, len_t pattern_size, len_t total_size);
    )
    submodule(Math,
    	// 'element_size' MUST be a power of 2 (1, 2, 4, 8) to define the unit of arithmetic.
    	void* fn(add)(void* dest, const void* src1, const void* src2, len_t total_size, byte element_size);
    
    	// Element-wise subtraction of two buffers. *dest = *src1 - *src2.
    	void* fn(sub)(void* dest, const void* src1, const void* src2, len_t total_size, byte element_size);

    	// Element-wise multiplication of two buffers. *dest = *src1 * *src2.
    	void* fn(mul)(void* dest, const void* src1, const void* src2, len_t total_size, byte element_size);
    	
	// Element-wise division of two buffers. *dest = *src1 / *src2.
    	void* fn(div)(void* dest, const void* src1, const void* src2, len_t total_size, byte element_size);
    )
    submodule(Bits,
    
    	// Element-wise bitwise AND of two buffers. *dest = *src1 & *src2.
    	void* fn(and)(void* dest, const void* src1, const void* src2, len_t total_size, byte element_size);

    	// Element-wise bitwise OR of two buffers. *dest = *src1 | *src2.
    	void* fn(or)(void* dest, const void* src1, const void* src2, len_t total_size,  byte element_size);

    	// Element-wise bitwise XOR of two buffers. *dest = *src1 ^ *src2.
    	void* fn(xor)(void* dest, const void* src1, const void* src2, len_t total_size, byte element_size);
    )
  )
};



#endif

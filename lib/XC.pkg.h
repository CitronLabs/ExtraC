/*---------------------------------------------------------------------------
MIT License

Copyright (c) 2025 CitronLabs

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
---------------------------------------------------------------------------*/
#pragma once
#define __EXTRAC__

#include "config.c"

#include "XC.Env/pkg.h"

#define EXTRAC_STD_CORE_IMPORTS
#define EXTRAC_STD_DATA_IMPORTS
#define EXTRAC_STD_MATH_IMPORTS
#define EXTRAC_STD_TIME_IMPORTS
#define EXTRAC_STD_DEBUG_IMPORTS
#define EXTRAC_STD_MEDIA_IMPORTS
#define EXTRAC_STD_NETWORK_IMPORTS
#define EXTRAC_STD_SCHEDULER_IMPORTS
#define EXTRAC_STD_STORAGE_IMPORTS
#define EXTRAC_STD_USER_IMPORTS
#define EXTRAC_STD_IO_IMPORTS
#define EXTRAC_STD_SYSTEM_IMPORTS

#ifdef EXTRAC_STD_CORE
    #include "XC.Core/pkg.h"
    #undef EXTRAC_STD_CORE_IMPORTS
    #define EXTRAC_STD_CORE_IMPORTS		    \
	interface(std_Types)		Types;	    \
	interface(std_Error)		Error;	    \
	interface(std_Array) 		Array;	    \
	interface(std_Stream) 		Stream;	    \
	interface(std_Memory) 		Memory;	    
	
#endif
#ifdef EXTRAC_STD_DATA
    #include "XC.Data/pkg.h"
    #undef EXTRAC_STD_DATA_IMPORTS
    #define EXTRAC_STD_DATA_IMPORTS		\
	interface(std_Number) 		Number;	\
	interface(std_String) 		String;	\
	interface(std_List)   		List;	\
	interface(std_Map)    		Map;	\
	interface(std_Struct) 		Struct;	\
	interface(std_DSN)    		DSN;	\
	interface(std_Editor) 		Editor;	
#endif
#ifdef EXTRAC_STD_DEBUG
    #include "XC.Debug/pkg.h"
    #undef EXTRAC_STD_DEBUG_IMPORTS
    #define EXTRAC_STD_DEBUG_IMPORTS		
#endif
#ifdef EXTRAC_STD_MATH
    #include "XC.Math/pkg.h"
    #undef EXTRAC_STD_MATH_IMPORTS
    #define EXTRAC_STD_MATH_IMPORTS		\
	interface(std_Vector) 		Vector;	\
	interface(std_Math) 		Math;	
#endif
#ifdef EXTRAC_STD_ALLOC
   #include "XC.Alloc/pkg.h"
   #undef EXTRAC_STD_ALLOC_IMPORTS
   #define EXTRAC_STD_ALLOC_IMPORTS		\
	interface(std_Arena) 		Arena;	\
	interface(std_Pool) 		Pool;	\
	interface(std_Buffer) 		Buffer;
#endif
#ifdef EXTRAC_STD_TIME
    #include "XC.Time/pkg.h"
    #undef EXTRAC_STD_TIME_IMPORTS
    #define EXTRAC_STD_TIME_IMPORTS		\
	interface(std_Time) 		Time;	\
	interface(std_Date) 		Date;	\
	interface(std_Watch) 		Watch;	\
	interface(std_Clock) 		Clock;
#endif
#ifdef EXTRAC_STD_MEDIA
    #include "XC.Media/pkg.h"
    #undef EXTRAC_STD_MEDIA_IMPORTS
    #define EXTRAC_STD_MEDIA_IMPORTS		  \
	interface(std_Graphics) 	Graphics; \
	interface(std_Audio) 		Audio;	  \
	interface(std_Media) 		Media;	  
#endif
#ifdef EXTRAC_STD_NETWORK
    #include "XC.Network/pkg.h"
    #undef EXTRAC_STD_NETWORK_IMPORTS
    #define EXTRAC_STD_NETWORK_IMPORTS		    \
	interface(std_Socket) 		Socket;     \
	interface(std_Connection) 	Connection; \
	interface(std_Network) 		Network;	  
#endif
#ifdef EXTRAC_STD_SCHEDULER
    #include "XC.Scheduler/pkg.h"
    #undef EXTRAC_STD_SCHEDULER_IMPORTS
    #define EXTRAC_STD_SCHEDULER_IMPORTS	\
	interface(std_Thread) 		Thread;	\
	interface(std_Process) 		Process; 
#endif
#ifdef EXTRAC_STD_STORAGE
	#include "XC.Storage/pkg.h"
    	#undef EXTRAC_STD_STORAGE_IMPORTS
    	#define EXTRAC_STD_STORAGE_IMPORTS	\
	interface(std_FS) 		FS;	
#endif
#ifdef EXTRAC_STD_USER
	#include "XC.User/pkg.h"
#endif
#ifdef EXTRAC_STD_IO
	#include "XC.IO/pkg.h"
    	#undef EXTRAC_STD_IO_IMPORTS
    	#define EXTRAC_STD_IO_IMPORTS		  \
	interface(std_Console) 		Console;  \
	interface(std_CLI) 		CLI;	  \
	interface(std_Input) 		Input;	  
#endif
#ifdef EXTRAC_STD_SYSTEM
	#include "XC.System/pkg.h"
    	#undef EXTRAC_STD_SYSTEM_IMPORTS
    	#define EXTRAC_STD_SYSTEM_IMPORTS	\
	interface(std_System) 		System;	
#endif


Module(std){ 
  values(Version, word,
	MAJOR,
	MINOR,
	PATCH
  )
	EXTRAC_STD_CORE_IMPORTS
	EXTRAC_STD_DATA_IMPORTS
	EXTRAC_STD_MATH_IMPORTS
	EXTRAC_STD_TIME_IMPORTS
	EXTRAC_STD_DEBUG_IMPORTS
	EXTRAC_STD_MEDIA_IMPORTS
	EXTRAC_STD_NETWORK_IMPORTS
	EXTRAC_STD_SCHEDULER_IMPORTS
	EXTRAC_STD_STORAGE_IMPORTS
	EXTRAC_STD_USER_IMPORTS
	EXTRAC_STD_IO_IMPORTS
	EXTRAC_STD_SYSTEM_IMPORTS
};


#undef EXTRAC_STD_CORE_IMPORTS
#undef EXTRAC_STD_DATA_IMPORTS
#undef EXTRAC_STD_MATH_IMPORTS
#undef EXTRAC_STD_TIME_IMPORTS
#undef EXTRAC_STD_DEBUG_IMPORTS
#undef EXTRAC_STD_MEDIA_IMPORTS
#undef EXTRAC_STD_NETWORK_IMPORTS
#undef EXTRAC_STD_SCHEDULER_IMPORTS
#undef EXTRAC_STD_STORAGE_IMPORTS
#undef EXTRAC_STD_USER_IMPORTS

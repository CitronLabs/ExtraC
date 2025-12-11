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
#ifndef __XC_CORE__
#define __EXTRAC_CORE__


#include "Essential/pkg.h"
#include "Debug/pkg.h"
#include "Data/pkg.h"
#include "Strings/pkg.h"
#include "Math/pkg.h"
#include "Alloc/pkg.h"
#include "Time/pkg.h"

Module(std){ 
  values(Version, word,
	MAJOR,
	MINOR,
	PATCH
  )
	interface(std_Types)		Types;
	interface(std_Error)		Error;
	interface(std_Local) 	  	Local;
	interface(std_Array) 		Array;
	interface(std_Stream) 		Stream;
	interface(std_FS) 		FS;
   	interface(std_String) 		String;
   	interface(std_Memory) 		Memory;
	interface(std_List)   		List;
	interface(std_Map)    		Map;
	interface(std_Struct) 		Struct;
	interface(std_DSN)    		DSN;
   	interface(std_StringBuilder) 	StringBuilder;
   	interface(std_Arena) 		Arena;
   	interface(std_Pool) 		Pool;
   	interface(std_Buffer) 		Buffer;
   	interface(std_Logger) 		Logger;
   	interface(std_Time) 		Time;
   	interface(std_Date) 		Date;
   	interface(std_Watch) 		Watch;
   	interface(std_Clock) 		Clock;
   	interface(std_Number) 		Number;
   	interface(std_Vector) 		Vector;
   	interface(std_Math) 		Math;
};

#endif

/*--------------------------------------------------------------|
 *								|
 * 			  Documentation 			|
 *								|
 -------------------------------------------------------------*/
/**
 * \ingroup XC-Package 
 * \defgroup xc-core-package Core
 *
 * @section xc-core Core Package
 * ***DEPENDENCY LEVEL: 0*** \n 
 * *Last Updated: September 11, 2025*
 *
 * @subsection desc Brief Description
 * ExtraC's core package contains the core set of features used by 
 * all other ExtraC's packages, many of the features within this package
 * act as simple utilities over things such as strings, memory allocation, 
 * error handling, and timing. It also implements common software development concepts
 * such as, classes/objects, data structures, vtables, serialization, etc. 
 * ***(see below for more information)***
 *
 * @subsection feat Features
 * - **Type Features:**
 *   - Classes and Objects
 *   - Interfaces and Modules
 *   - Heap-Memory Construction/Destruction via. **new/del** macros
 *   - Stack-Memory Construction/Destruction via. **push/pop** macros
 *   - Base-Type Redefinitions
 *
 * - **Data Features:**
 *   - *Data Structures*:
 *  	- List (Dynamic ArrayList)
 *   	- Stack
 *   	- Queue
 *   	- Map
 *   	- Struct (Dynamic Runtime Structures)
 *   - Binary/Text Serialization via. DSN class
 *   - Data Iteration via. IterableList interface and **foreach()** macro
 *
 * - **Error Features:**
 *   - Error Codes
 *   - Logging via. Logger and LogBook classes
 *   - *Handling Macro/Utilities*:
 *   	- Signal Handling
 *   	- Value Passing
 *   	- Try-Catch statements
 *
 * - **Allocation Features:**
 *   - Allocator Interface
 *   - Buffer utilities
 *   - *Common Allocators*:
 *   	- Arena 
 *   	- Pool
 *   - Heap Allocation Tracking via. CMalloc wrapper class
 *
 * - **Math Features:**
 *   - Arbitrary Length integer/ floating-point numbers via. Number class
 *   - Arbitrary Length Numbers Operations
 *   - Vectors and Matrixes
 *   - SIMD Operations
 *
 * - **String Features:**
 *   - UTF8 - 32 Handling, Storage, and Conversion
 *   - Building via. StringBuilder class 
 *   - Interpolation and Scanning via. Formatter interface and **$()** macro
 *   - *Manipulation*:
 *   	- Concatination
 *   	- Cutting
 *   	- Copying
 *   - Regex and Comparision
 *
 * - **Timing Features:**
 *   - *Time Range Format Representations:*
 *   	- TimeShort
 *   	- TimeLong
 *   	- TimeTiny
 *   - Timing via. TimeWatch class
 *   - Conversion Macros
*/

#undef package

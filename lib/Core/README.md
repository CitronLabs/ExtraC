***[Home](https://github.com/CitronLabs/ExtraC/tree/dev) > [Packages](https://github.com/CitronLabs/ExtraC/tree/dev/lib) > [Core](https://github.com/CitronLabs/ExtraC/tree/dev/lib/Core)***

<p align="center">
 <img src="https://github.com/CitronLabs/CitronLabs-SDK/blob/main/__media/ExtraC-CorePackage-Banner_1x.png">
</p>



## Overview
ExtraC's core package include all of the major features and additions used by all other packages, in other words for the most core and basic experience this where you'll find it.

Additionally this package can be futher subdivided in seperate subsets using the [config file](https://github.com/CitronLabs/ExtraC/tree/dev/build/config) for more control over 
the size and functionality of the resulting library file

## 🔗 Dependencies 
  - A modern **C11 or above compiler** with support for **"typeof()"** operator. Ex. GCC, Clang, etc...
  - A **C11 or above** compliant implementation of **libc**

## 🔨 How to build
***Using doBuild Script:***
  #### On Posix:
  ``` shell
  > cd build
  > ./doBuild.sh --package core 
  ```
  #### On Windows:
  ``` shell
  > cd build
  > ./doBuild.ps1 --package core
  ```

## 📦 Features
- **Basic Features:**
    - Classes and Objects
    - Modules
    - Data Streams
    - Error codes and basic handling
    - Atomics and thread safety wrappers and utils
    - *Operators*:
      - Data Iteration via. **foreach()** macro
      - Data Manip  via. **append(), get(), set(), copy() and hash()** macros
      - Allocation and Deallocation:
        - Heap-Memory via. **new/del** macros
        - Stack-Memory via. **push/pop** macros
      - Data Formatting and Parsing via. **print() and scan()** macros
    - More robust base types and primitives definitions
    - SIMD operation wrappers
    - Utility macros
 
 - **Data Features:**
    - *Data Structures*:
 	    - List (Dynamic Array)
  	  - Stack
      - Queue
      - Map
      - Struct (Dynamic Runtime Structures)
    - Binary/Text Serialization via. DSN class
    - 
 
- **Error Features:**
  - Error Codes
  - Logging via. Logger and LogBook classes
  - *Handling Macro/Utilities*:
       - Signal Handling
    	 - Value Passing
    	 - Try-Catch statements
 
 - **Allocation Features:**
   - Allocator Interface
   - Buffer utilities
   - *Common Allocators*:
    	  - Arena 
    	  - Pool
   - Heap Allocation Tracking via. CMalloc wrapper class
 
- **Math Features:**
   - Arbitrary Length integer/ floating-point numbers via. Number class
   - Arbitrary Length Numbers Operations
   - Vectors and Matrixes
   - SIMD Operations
 
- **String Features:**
   - UTF8 - 32 Handling, Storage, and Conversion
   - Building via. StringBuilder class 
   - Interpolation and Scanning via. Formatter interface and **$()** macro
   - *Manipulation*:
      - Concatination
   	  - Cutting
      - Copying
   - Regex and Comparision
 
- **Timing Features:**
   - *Time Range Format Representations:*
    	- TimeShort
    	- TimeLong
    	- TimeTiny
 *   - Timing via. TimeWatch class
 *   - Conversion Macros

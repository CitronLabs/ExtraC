#pragma once
#define __XC_IO_CONSOLE__
#include "pkg.h"

#define print(...) 	 printTo(std.Console.getDefault()->output,  __VA_ARGS__) 
#define scan(...) 	 scanFrom(std.Console.getDefault()->input,  __VA_ARGS__) 
#define println(...) 	 printTo(std.Console.getDefault()->output, __VA_ARGS__ , "\n") 
#define scanln(...) 	 scanFrom(std.Console.getDefault()->input, __VA_ARGS__ , "\n") 

#define printErr(...) 	 printTo(std.Console.getDefault()->error, __VA_ARGS__) 
#define printlnErr(...)  printTo(std.Console.getDefault()->error, __VA_ARGS__ , "\n") 


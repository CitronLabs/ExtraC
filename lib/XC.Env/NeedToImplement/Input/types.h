#pragma once
#define __XC_OS_SYSTEM_INPUT__
#include "pkg.h"

#define package os_Sys_Input

typefrom(pntr, Handle)


#define InputHandle 	os_Sys_Input_Handle
#define InputDevice 	os_Sys_Input_Device
#define InputInfo 	os_Sys_Input_Info
#define InputData 	os_Sys_Input_Data

typefrom(word, Type)
typefrom(word, EventType)

type(Device,
	std_String 
     		name,
     		vendor,
     		product;

	pntr uniqueID;

     	Array(InputInfo)    inputs;
)


type(Info,
	os_Sys_Input_Type type;
     	union{
	  struct {
		double 	low;
		double 	high;
		u8 	dimension  : 2;
          } position;
	  struct {
     		std_String 	     locale;	
		std_String_Encoding  encoding;
     	  } key;
     	  struct {
		len_t states, low, high;
     	  } button;
     	} data;
)

type(Data,
	os_Sys_Input_Type type;
     	union{
	  struct {
     		float pos[3]; 
          } position;
	  struct {
     		rune code; 
     		bool continues;
     	  } key;
     	  struct {
		len_t state;
     	  } button;
     	} data;
)

type(Event,
     	InputData   input;	
	InputHandle handle;
     	os_Sys_Input_EventType type;
)

#undef  package

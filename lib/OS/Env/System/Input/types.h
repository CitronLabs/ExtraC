#pragma once
#define __XC_OS_ENV_SYSTEM_INPUT__
#include "pkg.h"

#define package os_Env_Input

typefrom(pntr, Handle)

type(Vec3D, float x,y,z;);
type(Vec2D, float x,y;);
type(Vec1D, float x;);

#define Vec3D 	 	os_Env_Input_Vec3D
#define Vec2D 	 	os_Env_Input_Vec2D
#define Vec1D 	 	os_Env_Input_Vec1D
#define PosInput 	os_Env_Input_Position_Input
#define KeyInput 	os_Env_Input_Key_Input
#define InputHandle 	os_Env_Input_Handle
#define InputDevice 	os_Env_Input_Device


type(Device,
	std_String 
     		name,
     		vendor,
     		product;

	pntr uniqueID;

     	Array(PosInput) posInputs;
     	Array(KeyInput) keyInputs;
)


#undef  package




#define package os_Env_Input_Position
#define PosData os_Env_Input_Position_Data
#define PosType os_Env_Input_Position_Type

type(Data, 
     Vec3D pos; 
     float max, min;
)

typefrom(word, Type)

type(Input,
	u8 	dimension  : 2;
     	PosType type 	   : 2;
	float 	low;
	float 	high;
)
	  


Interface(Device,
	PosData 	imethod(get);	
	errvt 		imethod(update, PosData pos);
	InputHandle 	imethod(getHandle);
)

#undef package
#define package os_Env_Input_Key
#define KeyData os_Env_Input_Key_Data

type(Input,
     	std_String 	     locale;	
	std_String_Encoding  encoding;
)

type(Data, u32 code; bool cont;)

Interface(KeyDevice,
	KeyData     imethod(get);
	InputHandle imethod(getHandle);
)

#undef package



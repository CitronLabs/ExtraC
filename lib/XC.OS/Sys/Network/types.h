#pragma once
#define __XC_OS_SYSTEM_NETWORK__
#include "pkg.h"

#define package os_Sys_Network
#define NetworkHandle os_Sys_Network_Handle
#define NetworkDevice os_Sys_Network_Device

typefrom(pntr, Handle)

type(Device,
	std_String name;
	std_String manufacturer;
	std_String model;

	void* uniqueID;

	u8 mac_address[6]; 	
	bool up; 		
)



typefrom(word, Device_EventType)

type(Device_Event,
    NetworkHandle handle;
    os_Sys_Network_Device_EventType type;
)


#undef package
#define package os_Sys_Network_Socket

typefrom(word, Type)

type(Address_IPV4,
    u8  value[4];
    u16 port;
)

type(Address_IPV6,
    u16 value[8];
    u16 port;
)

type(Address_Local,
    std_String path;
)


type(Address,
     	os_Sys_Network_Socket_Type type;
	union {
		os_Sys_Network_Socket_Address_IPV4  ipv4;
		os_Sys_Network_Socket_Address_IPV6  ipv6;
		os_Sys_Network_Socket_Address_Local local;
     	}address;
);

typefrom(word, EventType)


type(Event,
    NetworkHandle handle;
    os_Sys_Network_Socket_EventType type;
)


#undef package
#define package os_Sys_Network_Object


type(Method,
	const c8*   	 name; 
     	Buffer(Type_t*)  parameters;

)

type(Field,
	const c8*      	 name; 
	std_varData   	 field;
)

type(Info,
     	const c8
     		* interface,
     		* name;
	Buffer(os_Sys_Network_Object_Method*) methods;
	Buffer(os_Sys_Network_Object_Field*)  field;
)

typefrom(word, EventType)

type(Event,
    NetworkHandle handle;
    os_Sys_Network_Object_EventType type;
)

#undef package

#pragma once
#define __XC_OS_SYSTEM__
#include "../pkg.h"


#ifndef __XC_OS_SYSTEM_NETWORK__

#include "types.h"
#define SocketType 	os_Sys_Network_Socket_Type
#define SocketAddress 	os_Sys_Network_Socket_Address
#define NetObjInfo 	os_Sys_Network_Object_Info

#define package os_Sys

Interface(Network,
	values(Version, word,
		MAJOR,
		MINOR,
		PATCH
	)
	errvt 		fn(initSystem);
	errvt 		fn(exitSystem);
  submodule(Socket,
	values(Type, SocketType,
		LOCAL,
		IPV4,
		IPV6,
		TCP,
		UDP,
		RAW 
	)
	NetworkHandle 	fn(init)	(SocketType    type);	
	errvt 		fn(bind)	(NetworkHandle handle, SocketAddress* address);
	errvt 		fn(listen)	(NetworkHandle handle, u32 num_connect);
	NetworkHandle 	fn(connect)	(SocketType type,      SocketAddress* address);
	NetworkHandle	fn(accept)	(NetworkHandle handle);
	errvt 		fn(send)	(NetworkHandle handle, std_Buffer* message);
	errvt 		fn(recv)	(NetworkHandle handle, std_Buffer* message);
    submodule(Group,
	errvt 		fn(join)	(NetworkHandle handle, SocketAddress address, strc8 interface_name);
	errvt 		fn(leave)	(NetworkHandle handle);
	errvt 		fn(send)	(NetworkHandle handle, std_Buffer* message);
	errvt 		fn(recive)	(NetworkHandle handle, std_Buffer* message);
    )
  )
  submodule(Object,
	const bool implemented; // for compatibiliy reasons
	
	values(Flag, word,
		ASYNC
  	)

	errvt 		fn(init)	(NetObjInfo* blueprint);
	errvt 		fn(getInfo)	(strc8 path, NetObjInfo* info);
	errvt 		fn(implement)	(NetObjInfo* blueprint);
	NetworkHandle 	fn(find)	(strc8 interface, strc8 object);
	std_DSN_Data 	fn(call)	(NetworkHandle object, word flags, strc8 method, std_DSN_Data* args);
	std_DSN_Data 	fn(get)		(NetworkHandle object, word flags, strc8 field);
	errvt 		fn(set)		(NetworkHandle object, word flags, strc8 field,  std_DSN_Data value);
	errvt 		fn(close)	(NetworkHandle handle);
  )
  submodule(Device,
	NetworkHandle 	fn(grab)	(NetworkDevice* device);
	errvt 		fn(drop)	(NetworkHandle handle);
	NetworkDevice* 	fn(enumerate)	(u64* numDevices);
	errvt		fn(filter);
	errvt		fn(send)	(std_Buffer* message);
	errvt		fn(recv)	(std_Buffer* message);
	errvt		fn(wait)	(std_Buffer* message);
  )
	values(Event, word,
    		NEWCLIENT,
    		RECIVE,
    		CLOSE
	)
	errvt 	  	fn(handleEvents)(NetworkHandle handle, ArrayQueue(OSEvent) evntQueue);
	u64 	  	fn(pollEvents);
)

#undef package
#undef SocketAddress
#undef SocketType
#undef NetworkHandle
#undef NetworkDevice
#undef NetObjInfo
#endif

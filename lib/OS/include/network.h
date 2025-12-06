#pragma once
#include "./extern.h"


typedef void* networkHandle;

typedef u8 ipv4_netaddress[4];
typedef u16 ipv6_netaddress[8];
typedef int socketType;

type(socketAddress,
     	socketType type;
	union {
	    struct ipv4Address{
		u16 port;
		ipv4_netaddress address;
     	    }ipv4;
	    struct ipv6Address{
		u16 port;
		ipv6_netaddress address;
     	    }ipv6;
	    struct localAddress{
		inst(String) path;
     	    }local;
     	}data;
);

typedef struct ipv4Address ipv4Address;
typedef struct ipv6Address ipv6Address;
typedef struct localAddress localAddress;

enum(netCall_Flags,
	netCall_Async = 1
)

#define netField(name, access, type, data) (netobjFieldInfo){s(name),NETFIELD_##access,(DSN_Data){DSN_##type, data}}
#define netMethodImpl(name, implModule, ...) (netobjMethodInfo){s(name),(DSN_Data){DSN_STRUCT, newStruct(__VA_ARGS__)}, &implModule, implModule##_Logic}
#define netMethodDecl(name, ...) (netobjMethodInfo){s(name),(DSN_Data){DSN_STRUCT, newStruct(__VA_ARGS__)}, null, NULL}

Blueprint(NetObjMethod,
__IO(in_DSN_data parameters; out_DSN_data returnvalue), 
__DATA()
)

type(netobjMethodInfo, 
	inst(String) name; 
     	DSN_data parameters;
     	modl(Module) module; 
     	moduleLogic impl;
);

#define NETFIELD_GET 1
#define NETFIELD_SET 2
#define NETFIELD_GET_SET (NETFIELD_GET | NETFIELD_SET)

type(netobjFieldInfo,
	inst(String) name; 
     	u16 access;
	DSN_data data;
)
type(netobjInfo,
     	data(String)
     	* interface,
     	* name;
	Buffer(networkMethodInfo) methods;
	Buffer(networkFieldInfo)  field;
)


type(networkDevice,
	inst(String) name;
	inst(String) manufacturer;
	inst(String) model;

	void* uniqueID;

	u8 mac_address[6]; 	
	bool up; 		
)

Interface(network,
	const strc8 stdVersion;
	errvt 		vmethod(initSystem);
	errvt 		vmethod(exitSystem);
	submodule(socket,
	  submodule(type,
	     	socketType
		LOCAL,
		IPV4,
		IPV6,
		TCP ,
		UDP ,
		RAW ;
	  )
	networkHandle 	vmethod(init,  		socketType   type);	
	errvt 		vmethod(bind, 		networkHandle handle, socketAddress* address);
	errvt 		vmethod(listen,     	networkHandle handle, u32 num_connect);
	networkHandle 	vmethod(connect,    	socketType type, socketAddress* address);
	networkHandle	vmethod(accept,     	networkHandle handle);
	errvt 		vmethod(send, 	    	networkHandle handle, inst(Buffer) message);
	errvt 		vmethod(recv,    	networkHandle handle, inst(Buffer) message);
	submodule(group,
	errvt 		vmethod(join,  		networkHandle handle, socketAddress address, const char* interface_name);
	errvt 		vmethod(leave,   	networkHandle handle);
	errvt 		vmethod(send,    	networkHandle handle, inst(Buffer) message);
	errvt 		vmethod(recive,  	networkHandle handle, inst(Buffer) message);
	)
	)
	submodule(obj,
	const bool implemented; // for compatibiliy reasons
	errvt 		vmethod(init,    netobjInfo* blueprint);
	errvt 		vmethod(getInfo,   strc8 path, netobjInfo* info);
	errvt 		vmethod(implement, netobjInfo* blueprint);
	networkHandle 	vmethod(find,  strc8 interface, strc8 object);
	DSN_data 	vmethod(call,  networkHandle object, netCall_Flags flags, strc8 method, DSN_data* args);
	DSN_data 	vmethod(get,   networkHandle object, netCall_Flags flags, strc8 field);
	errvt 		vmethod(set,   networkHandle object, netCall_Flags flags, strc8 field,  DSN_data value);
	errvt 		vmethod(close, networkHandle handle);
	)
	submodule(device,
	networkHandle 	vmethod(grab, 	 	networkDevice* device);
	errvt 		vmethod(drop, 	 	networkHandle);
	networkDevice* 	vmethod(enumerate, 	u64* numDevices);
	errvt		vmethod(filter)
	errvt		vmethod(send,		inst(Buffer) message);
	errvt		vmethod(recv,		inst(Buffer) message);
	errvt		vmethod(wait,		inst(Buffer) message);
	)
	errvt 	  	vmethod(handleEvents,  	 networkHandle handle, Queue(OSEvent) evntQueue);
	u64 	  	vmethod(pollEvents);
)
enum(SocketEventType,
    SocketEvent_NewClient,
    SocketEvent_Recive,
    SocketEvent_Close
)

type(SocketEvent,
    networkHandle handle;
    SocketEventType type;
)



enum(NetObjEventType,
    NetObjEvent_NewClient,
    NetObjEvent_Recive,
    NetObjEvent_Close
)
type(NetObjEvent,
    networkHandle handle;
    NetObjEventType type;
)


enum(NetDeviceEventType,
    NetDeviceEvent_NewClient,
    NetDeviceEvent_Recive,
    NetDeviceEvent_Close
)
type(NetDeviceEvent,
    networkHandle handle;
    NetDeviceEventType type;
)



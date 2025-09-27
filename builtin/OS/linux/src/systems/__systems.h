#pragma once
#include "../../include/extern.h"
#include "../../include/devices.h"
#include "../../include/events.h"
#include "../../include/utils.h"

enum(NetData_Type,
	NetData_Socket,
	NetData_Object,
	NetData_Device,
)

typedef struct netObjectData netObjectData;
typedef struct netObjectForeignData netObjectForeignData;
typedef struct netObjectRemoteData netObjectRemoteData;

struct(NetData_Object_Data,
	netObjectData* data;   
	netObjectForeignData* dataForeign;   
	netObjectRemoteData* dataRemote;   
     	u8 foreign : 1, remote : 1;
)

struct(NetData_Socket_Data,
	int fd;
	struct sockaddr address, group_address;
	socklen_t sizeofaddr;
	u8 protocal : 2;
	u8 domain   : 2;
	u8 blocking : 1;
)
struct(NetData_Device_Data,


)
struct(LinuxNetworkHandle,
	NetData_Type type;
     	union {
     		NetData_Socket_Data socket;
     		NetData_Object_Data object;
     		NetData_Device_Data device;
     	} data;
)

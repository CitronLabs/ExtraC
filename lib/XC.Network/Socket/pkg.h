#pragma once
#define __XC_NETWORK__
#include "../pkg.h"


#ifndef __XC_NETWORK_SOCKET__

Class(Connection,
INIT(
	u8 protocol;
	std_Sys_Network_Socket_Address address;
), 
FMT(),
	std_Sys_Network_Socket_Address address;
	std_Stream stream;
	u8 protocol;

private(
	bool isGroup;
	std_Sys_Network_Handle handle;
)
){
	std_Connection* 	method(Connection, joinGroup,
	      	std_Sys_Network_Socket_Address* address,
	      	std_Sys_Network_Socket_Address* interface_addr,
		std_Connection* alloc
	);
};

Class(Socket,
INIT(
      	u8 protocol;
      	std_Sys_Network_Socket_Address address;
),
FMT(),
      	std_Sys_Network_Socket_Address address;
	u8 protocol;

private(
      	std_Sys_Network_Handle handle;
)
){
	errvt method(Socket, bind);
	errvt method(Socket, listen, u32 num_waiting);
};







constructor(Connection, FromSocket, std_Socket* socket)

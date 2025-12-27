#pragma once
#define __XC_OS__
#include "../pkg.h"
#include "../Env/pkg.h"

#define package os



Class(Socket,
INIT(
      	u8 protocol;
      	os_Env_Network_Socket_Address address;
),
FMT(),
      	os_Env_Network_Socket_Address address;
	u8 protocol;

private(
      	os_Env_Network_Handle handle;
)
){
	errvt method(Socket, bind);
	errvt method(Socket, listen, u32 num_waiting);
};






Class(Connection,
INIT(
	u8 protocol;
	os_Env_Network_Socket_Address address;
), 
FMT(),
	os_Env_Network_Socket_Address address;
	std_Stream stream;
	u8 protocol;

private(
	bool isGroup;
	os_Env_Network_Handle handle;
)
){
	os_Connection* 	method(Connection, joinGroup,
	      	os_Env_Network_Socket_Address* address,
	      	os_Env_Network_Socket_Address* interface_addr,
		os_Connection* alloc
	);
};

constructor(Connection, FromSocket, os_Socket* socket)



constructor(Env_Network_Socket_Address, IPV4,  u8    address[4]; u16 port)
constructor(Env_Network_Socket_Address, IPV6,  u16   address[8]; u16 port)
constructor(Env_Network_Socket_Address, Local, strc8 path)


Interface(Network, 
	values(Protocol, word,
		TCP,
		UDP,
		RAW
	)

	values(Domain, word,
		IPV4,
		IPV6,
		LOCAL
	)
)

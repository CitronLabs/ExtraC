
#define P_null  0x00
#define P_TCP   0x01
#define P_UDP   0x02
#define P_RAW   0x03

#define D_null  0x00
#define D_IPV4  0x01
#define D_IPV6  0x02
#define D_LOCAL 0x03

#define Protocol(name, ...) 					\
	typedef struct{ __VA_ARGS__ } name##_header; 		\
	inline Packet msg##name(name##_header header, Packet body)

#define msgHandler(protocol, ...) 				\
	inline Packet msg##name(name##_header header, Packet body)

#define msgBuild(packet, ...){ 			\
	Packet __parent_packet = {0};		\
		__VA_ARGS__; 			\
	packet = __parent_packet;		\
} 
#define msgSend(conn, ...){ 			\
	Packet __parent_packet = {0};		\
		__VA_ARGS__; 			\
	Connection.Send(conn, __parent_packet);	\
} 

#define msgReturn(...){			 	\
	Packet __parent_packet = {0};		\
		__VA_ARGS__; 			\
	return __parent_packet;			\
}

#define msgBuff(buff, size) (Packet){size,buff}
#define msgString(string) __parent_packet = (Packet){strnlen(string, UINT64_MAX), string}; 
#define msgText(...) msgString(#__VA_ARGS__)
#define msg(protocol, ...)	 		\
	 Packet* protocol##_packet 		\
		 = &__parent_packet; 		\
	for(Packet __parent_packet = {0};	\
	    protocol##_packet->size == 0; 	\
	    *protocol##_packet = msg##protocol(	\
		(protocol##_header){__VA_ARGS__}\
		, __parent_packet))






#define NetClass(name, interface, FIELD, ...)
	  (Class, #name", interface,
	    b(FIELD),
	    b(__VA_ARGS__)
	  )


#define FIELD(...) __VA_ARGS__


#define NetField(name, type) push(Field, #name, type##_Type)







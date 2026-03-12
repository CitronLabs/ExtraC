
#define P_null  0x00
#define P_TCP   0x01
#define P_UDP   0x02
#define P_RAW   0x03

#define D_null  0x00
#define D_IPV4  0x01
#define D_IPV6  0x02
#define D_LOCAL 0x03

#define NetClass(name, interface, FIELD, ...)
	  (Class, #name", interface,
	    b(FIELD),
	    b(__VA_ARGS__)
	  )


#define FIELD(...) __VA_ARGS__


#define NetField(name, type) push(Field, #name, type##_Type)







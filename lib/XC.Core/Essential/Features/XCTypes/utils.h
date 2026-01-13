
#define getTypePtr(var) 			\
(_Generic((var),				\
strc8:   	strc8_Type,			\
strc16:  	strc16_Type,			\
strc32:  	strc32_Type,			\
u8:      	u8_Type,			\
u16:     	u16_Type,			\
u32:     	u32_Type,			\
u64:     	u64_Type,			\
i8:      	i8_Type,			\
i16:     	i16_Type,			\
i32:     	i32_Type,			\
i64:     	i64_Type,			\
std_Error*:     std_Error_Type,			\
default: (*((std_Object**)			\
	&(typeof(var)){var}))->__type		\
))				

#define asObject(var) (&(std_Object){generic getTypePtr(var)})


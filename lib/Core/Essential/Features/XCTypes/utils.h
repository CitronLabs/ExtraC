#define asObject(var)					\
_Generic((var), 					\
i32:    	(&(std_Object){generic i32_Type}),	\
u32:		(&(std_Object){generic u32_Type}), 	\
i64:    	(&(std_Object){generic i64_Type}), 	\
u64:    	(&(std_Object){generic u64_Type}), 	\
i16:   		(&(std_Object){generic i16_Type}), 	\
u16:   		(&(std_Object){generic u16_Type}),	\
i8:   		(&(std_Object){generic i8_Type}),	\
u8:   		(&(std_Object){generic u8_Type}),	\
float:   	(&(std_Object){generic float_Type}),	\
double:   	(&(std_Object){generic double_Type}),	\
const char*: 	(&(std_Object){generic strc8_Type}),	\
char*:	 	(&(std_Object){generic strc8_Type}),	\
char:	 	(&(std_Object){generic c8_Type}),	\
void*:	  	(&(std_Object){generic pntr_Type}),	\
bool:		(&(std_Object){generic bool_Type}), 	\
default: 	var)	


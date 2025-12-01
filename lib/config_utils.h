typedef unsigned long long 	value_config;
typedef double 			deci_config;
typedef long long 		int_config;
typedef unsigned int 		setting_config;
typedef unsigned char 		bool_config;


#define true  1
#define false 0

#define configMax(type)				\
	_Generic((type##_config){0},		\
	value_config: (~(value_config){0}),	\
	int_config:   ((~(int_config){0})>>1),  \
	deci_config:  1.7976931348623157e+308,	\
	setting_config: (~(setting_config){0}),	\
	bool_config:	true			\
	)

#define configMin(type)				\
	_Generic((type##_config){0},		\
	value_config: (value_config){0},	\
	int_config:   ~(configMax(int_config)),	\
	deci_config:  2.2250738585072014e-308,	\
	setting_config: (setting_config){0},	\
	bool_config:	false			\
	)

#define isSet(flags, flag) ((flags & flag) == flag)



#if defined( 			__TYPE_OPERATORS__			)
/*---------------------------------------------------------------------------*/
#ifndef OPERATOR
#define OPERATOR(class, name, returnval, ...)
#endif	

	#define OPERATOR_FUNCS(Class)							\
	    OPERATOR(Class, Create, Class*, len_t size, argsof(Class)* args);		\
	    OPERATOR(Class, Destroy,errvt);						\
	    OPERATOR(Class, Hash,   u32);						\
	    OPERATOR(Class, Write,  u64,     pntr data[], len_t size);			\
	    OPERATOR(Class, Read,   u64,     pntr data[], len_t size);			\
	    OPERATOR(Class, Set,    errvt,   void* value);				\
	    OPERATOR(Class, Get,    errvt,   void* buff);				\
	    OPERATOR(Class, Copy,   void*,   void* where);				\
	    OPERATOR(Class, Iter,   void*,   u64 index);				\
	    OPERATOR(Class, Size,   len_t,   bool elements);				\
	    OPERATOR(Class, Scan,   u64, fmtArgsOf(Class)* format, std_Stream* in);	\
	    OPERATOR(Class, Print,  u64, fmtArgsOf(Class)* format, std_Stream* out);	

#else
/*---------------------------------------------------------------------------*/


#endif

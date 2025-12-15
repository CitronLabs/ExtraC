#include <Core/pkg.c>
#include <float.h>
#include <stdio.h>
#include <wchar.h>

/*----------------------------------------------
 *						|
 *	      Floating-Point Operations		|
 *						|
-----------------------------------------------*/

#undef module
#define module double

PRINT(double){
	
	u64 formatted_len = 0;
	char* buff = alloca(snprintf(NULL, DBL_MAX_10_EXP + DBL_DIG, "%f", *self));
	
	if(buff)
		formatted_len = snprintf(NULL, DBL_MAX_10_EXP + DBL_DIG, "%f", *self);

	if(formatted_len != 0)
		write(out, buff);

return formatted_len;
}


SCAN(double){

	len_t scannedLen = 0; 

	rune c = 0, 
		buff[DBL_MAX_10_EXP + DBL_DIG + 1] = {0};


	std.Stream.Process
	    .start(in)
	    .doDecode(std.String.UTF8.Decoder, c){
		while(iswblank(c)) process->next();

		while(
			(iswdigit(c) || c == '.' || c == '-') && 
			scannedLen < DBL_MAX_10_EXP + DBL_DIG
		)
			{ buff[scannedLen++] = c; process->next(); }

		if(scannedLen == 0){ 
			ERR(ERR.INVALID, "invalid float syntax");
			process->fail();
			return 0; 
		}
		
		if(!swscanf(buff, L"%lf", self)){
			ERR(ERR.INVALID, "failed to parse double");
			process->fail();
			return 0;
		}
	    
	  } then.end()
	;

return scannedLen;
}


SIZE(double){	return sizeof(double); } 

COPY(double){ memcpy(where, self, sizeof(double)); return where; } 

HASH(double){ return hash_bytes(self, sizeof(double)); }

SET(double){ memcpy(self, value, sizeof(double)); return OK; }

DESTROY(double){ return OK; }

construct(double,
FMT(),
DEF(),
	.Scan  	 = double_Op_Scan,
	.Print 	 = double_Op_Print,
	.Create  = double_Op_Create,
	.Copy 	 = double_Op_Copy,
	.Size 	 = double_Op_Size,
	.Destroy = double_Op_Destroy,
	.Set	 = double_Op_Set,
	.Hash	 = double_Op_Hash,
	.Write   = nil,
	.Read	 = nil,
	.Iter 	 = nil,
){ passover }

#undef module
#define module float

PRINT(float){
	
	u64 formatted_len = 0;
	char* buff = alloca(snprintf(NULL, FLT_MAX_10_EXP + FLT_DIG, "%f", *self));
	
	if(buff)
		formatted_len = snprintf(buff, FLT_MAX_10_EXP + FLT_DIG, "%f", *self);

	if(formatted_len != 0)
		write(out, buff);

return formatted_len;
}

SCAN(float){
	len_t scannedLen = 0; 
	rune c = 0, 
		buff[FLT_MAX_10_EXP + FLT_DIG + 1] = {0};

	std.Stream.Process
	    .start(in)
	    .doDecode(std.String.UTF8.Decoder, c){
		while(iswblank(c)) process->next();

		while(
			(iswdigit(c) || c == '.' || c == '-') && 
			scannedLen < FLT_MAX_10_EXP + FLT_DIG
		)
			{ buff[scannedLen++] = c; process->next(); }

		if(scannedLen == 0){ 
			ERR(ERR.INVALID, "invalid float syntax");
			process->fail();
			return 0; 
		}
		
		if(!swscanf(buff, L"%f", self)){
			ERR(ERR.INVALID, "failed to parse float");
			process->fail();
			return 0;
		}
	    
	  } then.end()
	;

return scannedLen;
}

SIZE(float){ return sizeof(float); }

COPY(float) { memcpy(where, self, sizeof(float)); return where; }

HASH(float) { return hash_bytes(self, sizeof(float)); }

SET(float) { memcpy(self, value, sizeof(float)); return OK; }

DESTROY(float) { return OK; }

construct(float,
FMT(),
DEF(),
	.Scan  	 = float_Op_Scan,
	.Print 	 = float_Op_Print,
	.Create  = float_Op_Create,
	.Copy 	 = float_Op_Copy,
	.Size 	 = float_Op_Size,
	.Destroy = float_Op_Destroy,
	.Set	 = float_Op_Set,
	.Hash	 = float_Op_Hash,
	.Write   = nil,
	.Read	 = nil,
	.Iter 	 = nil,
){ passover }



/*----------------------------------------------
 *						|
 *	       Integer Operations		|
 *						|
-----------------------------------------------*/


#define u32_TXT_BUFF_SIZE 	11
#define i32_TXT_BUFF_SIZE 	12
#define u64_TXT_BUFF_SIZE 	21
#define i64_TXT_BUFF_SIZE 	21
#define i16_TXT_BUFF_SIZE 	7
#define u16_TXT_BUFF_SIZE 	6
#define i8_TXT_BUFF_SIZE  	5
#define u8_TXT_BUFF_SIZE  	4

#define len_t_TXT_BUFF_SIZE 	21
#define pntrval_TXT_BUFF_SIZE 	21

#define __PRINT_INT(type, formatter) 		\
PRINT(type) {					\
	char buff[type##_TXT_BUFF_SIZE] = {0}; 	\
	len_t written = snprintf		\
		(buff, type##_TXT_BUFF_SIZE - 1,\
   		formatter, *self); 		\
	if(written) write(out, buff); 		\
return written; 				\
}




#define __SCAN_INT(type, formatter) 		\
SCAN(type) {					\
	rune buff[type##_TXT_BUFF_SIZE] = {0}; 	\
	len_t					\
	  len_read = 				\
	__xctype_impl_fill_buff_from_stream	\
	(in, buff, type##_TXT_BUFF_SIZE),	\
	  scanned = swscanf			\
		(buff, 				\
   		formatter, self); 		\
	if(!scanned) return 0; 			\
return len_read; 				\
}

static inline len_t __xctype_impl_fill_buff_from_stream(std_Stream* in, rune* buff, len_t maxlen){
	rune c; 
	byte buff_cursor = 0;

	std.Stream.Process
	    .start(in)
	    .doDecode(std.String.UTF8.Decoder, c){

	    	while(iswblank(c)) process->next();

	    	if(!iswdigit(c)) {
	    		ERR(ERR.FAIL, "failed to scan for integer");
	    		process->fail();
	    		return 0;
	    	}

	    	while(iswdigit(c) || buff_cursor > maxlen - 1){
			buff[buff_cursor++] = c; process->next();
		}
	    	
		if(!iswblank(c)){
			ERR(ERR.FAIL, "invalid charaters at end of interger scan");
			process->fail();
			return 0;

		}
	    }
	    then.end()
	;

return buff_cursor;
}


#define __COPY_INT(type) 			\
COPY(type) {					\
	if(!memcpy			\
	   (where, self, sizeof(type)));	\
		return nil;			\
return where;					\
}


#define __SET_INT(type) 			\
SET(type) {					\
	if(!memcpy(self, value, sizeof(type)));	\
		return ERR(			\
	ERR.FAIL, "failed to set " #type);	\
return OK;					\
}


#define __HASH_INT(type) 			\
HASH(type) {					\
return hash_bytes(self, sizeof(type));		\
}

#define __SIZE_INT(type)			\
SIZE(type){ return sizeof(type); }



#define __DESTROY_INT(type)			\
DESTROY(type)					\
	{ return OK; }


#define __CONSTRUCT_INT(type)			\
construct(type,					\
FMT(),						\
DEF(),						\
	.Print   = type##_Op_Print,		\
	.Scan    = type##_Op_Scan,		\
	.Create  = type##_Op_Create,		\
	.Copy 	 = type##_Op_Copy,		\
	.Size 	 = type##_Op_Size,		\
	.Destroy = type##_Op_Destroy,		\
	.Set	 = type##_Op_Set,		\
	.Hash	 = type##_Op_Hash,		\
	.Read    = nil,			\
	.Write 	 = nil,			\
	.Iter 	 = nil,			\
){ passover }

#undef module
#define module pntrval

__PRINT_INT(pntrval, "%llu");
__SCAN_INT(pntrval, L"%u");
__COPY_INT(pntrval);
__SET_INT(pntrval);
__HASH_INT(pntrval);
__SIZE_INT(pntrval);
__DESTROY_INT(pntrval);
__CONSTRUCT_INT(pntrval);

#undef module
#define module len_t

__PRINT_INT(len_t, "%llu");
__SCAN_INT(len_t, L"%u");
__COPY_INT(len_t);
__SET_INT(len_t);
__HASH_INT(len_t);
__SIZE_INT(len_t);
__DESTROY_INT(len_t);
__CONSTRUCT_INT(len_t);

#undef module
#define module u8

__PRINT_INT(u8,   "%u");
__SCAN_INT(u8,   L"%u");
__COPY_INT(u8);
__SET_INT(u8);
__HASH_INT(u8);
__SIZE_INT(u8);
__DESTROY_INT(u8);
__CONSTRUCT_INT(u8);

#undef module
#define module u16

__PRINT_INT(u16,  "%u");
__SCAN_INT(u16,  L"%u");
__COPY_INT(u16);
__SET_INT(u16);
__HASH_INT(u16);
__SIZE_INT(u16);
__DESTROY_INT(u16);
__CONSTRUCT_INT(u16);

#undef module
#define module u32

__PRINT_INT(u32,  "%u");
__SCAN_INT(u32,  L"%u");
__COPY_INT(u32);
__SET_INT(u32);
__HASH_INT(u32);
__SIZE_INT(u32);
__DESTROY_INT(u32);
__CONSTRUCT_INT(u32);

#undef module
#define module u64

__PRINT_INT(u64,  "%llu");
__SCAN_INT(u64,  L"%u");
__COPY_INT(u64);
__SET_INT(u64);
__HASH_INT(u64);
__SIZE_INT(u64);
__DESTROY_INT(u64);
__CONSTRUCT_INT(u64);

#undef module
#define module i8

__PRINT_INT(i8,   "%i");
__SCAN_INT(i8,   L"%i");
__COPY_INT(i8);
__SET_INT(i8);
__HASH_INT(i8);
__SIZE_INT(i8);
__DESTROY_INT(i8);
__CONSTRUCT_INT(i8);

#undef module
#define module i16

__PRINT_INT(i16,  "%i");
__SCAN_INT(i16,  L"%i");
__COPY_INT(i16);
__SET_INT(i16);
__HASH_INT(i16);
__SIZE_INT(i16);
__DESTROY_INT(i16);
__CONSTRUCT_INT(i16);

#undef module
#define module i32

__PRINT_INT(i32,  "%i");
__SCAN_INT(i32,  L"%i");
__COPY_INT(i32);
__SET_INT(i32);
__HASH_INT(i32);
__SIZE_INT(i32);
__DESTROY_INT(i32);
__CONSTRUCT_INT(i32);

#undef module
#define module i64


__PRINT_INT(i64,  "%lli");
__SCAN_INT(i64,  L"%i");
__COPY_INT(i64);
__SET_INT(i64);
__HASH_INT(i64);
__SIZE_INT(i64);
__DESTROY_INT(i64);
__CONSTRUCT_INT(i64);

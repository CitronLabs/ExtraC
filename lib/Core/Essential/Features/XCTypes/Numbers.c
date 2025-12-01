#include "../../../pkg.h"
#include <float.h>
#include <stdio.h>
#include <wchar.h>

import(std)


/*----------------------------------------------
 *						|
 *	      Floating-Point Operations		|
 *						|
-----------------------------------------------*/

PRINT(double){
	
	u64 formatted_len = 0;
	char* buff = alloca(snprintf(null, DBL_MAX_10_EXP + DBL_DIG, "%f", self));
	
	if(buff != null)
		formatted_len = snprintf(buff, DBL_MAX_10_EXP + DBL_DIG, "%f", self);

	if(formatted_len != 0)
		write(out, buff);

return formatted_len;
}

PRINT(float){
	
	u64 formatted_len = 0;
	char* buff = alloca(snprintf(null, FLT_MAX_10_EXP + FLT_DIG, "%f", self));
	
	if(buff != null)
		formatted_len = snprintf(buff, FLT_MAX_10_EXP + FLT_DIG, "%f", self);

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
			ERR(ERR_INVALID, "invalid float syntax");
			process->fail();
			return 0; 
		}
		
		if(!swscanf(buff, L"%lf", self)){
			ERR(ERR_INVALID, "failed to parse double");
			process->fail();
			return 0;
		}
	    
	  } then.end()
	;

return scannedLen;
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
			ERR(ERR_INVALID, "invalid float syntax");
			process->fail();
			return 0; 
		}
		
		if(!swscanf(buff, L"%f", self)){
			ERR(ERR_INVALID, "failed to parse float");
			process->fail();
			return 0;
		}
	    
	  } then.end()
	;

return scannedLen;
}


SIZE(double){	return sizeof(double); } 
SIZE(float){ return sizeof(float); }

COPY(double){ memcpy(where, self, sizeof(double)); return where; } 
COPY(float) { memcpy(where, self, sizeof(float)); return where; }

HASH(double){ return hash_bytes(self, sizeof(double)); }
HASH(float) { return hash_bytes(self, sizeof(float)); }

SET(double){ memcpy(self, value, sizeof(double)); return OK; }
SET(float) { memcpy(self, value, sizeof(float)); return OK; }

DESTROY(double){ return OK; }
DESTROY(float) { return OK; }

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
	.Write   = nilmethod,
	.Read	 = nilmethod,
	.Iter 	 = nilmethod,
){ passover }

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
	.Write   = nilmethod,
	.Read	 = nilmethod,
	.Iter 	 = nilmethod,
){ passover }



/*----------------------------------------------
 *						|
 *	       Integer Operations		|
 *						|
-----------------------------------------------*/


#define u32_TXT_BUFF_SIZE 11
#define i32_TXT_BUFF_SIZE 12
#define u64_TXT_BUFF_SIZE 21
#define i64_TXT_BUFF_SIZE 21
#define i16_TXT_BUFF_SIZE 7
#define u16_TXT_BUFF_SIZE 6
#define i8_TXT_BUFF_SIZE  5
#define u8_TXT_BUFF_SIZE  4

#define len_t_TXT_BUFF_SIZE 21

#define __PRINT_INT(type, formatter) 		\
PRINT(type) {					\
	char buff[type##_TXT_BUFF_SIZE] = {0}; 	\
	len_t written = snprintf		\
		(buff, type##_TXT_BUFF_SIZE - 1,\
   		formatter, *self); 		\
	if(written) write(out, buff); 		\
return written; 				\
}

__PRINT_INT(len_t,"%llu");
__PRINT_INT(u8,   "%u");
__PRINT_INT(u16,  "%u");
__PRINT_INT(u32,  "%u");
__PRINT_INT(u64,  "%llu");
__PRINT_INT(i8,   "%i");
__PRINT_INT(i16,  "%i");
__PRINT_INT(i32,  "%i");
__PRINT_INT(i64,  "%lli");



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
	    		ERR(ERR_FAIL, "failed to scan for integer");
	    		process->fail();
	    		return 0;
	    	}

	    	while(iswdigit(c) || buff_cursor > maxlen - 1){
			buff[buff_cursor++] = c; process->next();
		}
	    	
		if(!iswblank(c)){
			ERR(ERR_FAIL, "invalid charaters at end of interger scan");
			process->fail();
			return 0;

		}
	    }
	    then.end()
	;

return buff_cursor;
}

__SCAN_INT(len_t,L"%u");
__SCAN_INT(u8,   L"%u");
__SCAN_INT(u16,  L"%u");
__SCAN_INT(u32,  L"%u");
__SCAN_INT(u64,  L"%u");
__SCAN_INT(i8,   L"%i");
__SCAN_INT(i16,  L"%i");
__SCAN_INT(i32,  L"%i");
__SCAN_INT(i64,  L"%i");


#define __COPY_INT(type) 			\
COPY(type) {					\
	if(!memcpy			\
	   (where, self, sizeof(type)));	\
		return null;			\
return where;					\
}

__COPY_INT(len_t);
__COPY_INT(u8);
__COPY_INT(u16);
__COPY_INT(u32);
__COPY_INT(u64);
__COPY_INT(i8);
__COPY_INT(i16);
__COPY_INT(i32);
__COPY_INT(i64);


#define __SET_INT(type) 			\
SET(type) {					\
	if(!memcpy(self, value, sizeof(type)));	\
		return ERR(			\
	ERR_FAIL, "failed to set " #type);	\
return OK;					\
}

__SET_INT(len_t);
__SET_INT(u8);
__SET_INT(u16);
__SET_INT(u32);
__SET_INT(u64);
__SET_INT(i8);
__SET_INT(i16);
__SET_INT(i32);
__SET_INT(i64);

#define __HASH_INT(type) 			\
HASH(type) {					\
return hash_bytes(self, sizeof(type));		\
}

__HASH_INT(len_t);
__HASH_INT(u8);
__HASH_INT(u16);
__HASH_INT(u32);
__HASH_INT(u64);
__HASH_INT(i8);
__HASH_INT(i16);
__HASH_INT(i32);
__HASH_INT(i64);

#define __SIZE_INT(type)			\
SIZE(type){ return sizeof(type); }


__SIZE_INT(len_t);
__SIZE_INT(u8);
__SIZE_INT(u16);
__SIZE_INT(u32);
__SIZE_INT(u64);
__SIZE_INT(i8);
__SIZE_INT(i16);
__SIZE_INT(i32);
__SIZE_INT(i64);


#define __DESTROY_INT(type)			\
DESTROY(type)					\
	{ if(self) return OK; }


__DESTROY_INT(len_t);
__DESTROY_INT(u8);
__DESTROY_INT(u16);
__DESTROY_INT(u32);
__DESTROY_INT(u64);
__DESTROY_INT(i8);
__DESTROY_INT(i16);
__DESTROY_INT(i32);
__DESTROY_INT(i64);

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
	.Read    = nilmethod,			\
	.Write 	 = nilmethod,			\
	.Iter 	 = nilmethod,			\
){ passover }


__CONSTRUCT_INT(len_t);
__CONSTRUCT_INT(u8);
__CONSTRUCT_INT(u16);
__CONSTRUCT_INT(u32);
__CONSTRUCT_INT(u64);
__CONSTRUCT_INT(i8);
__CONSTRUCT_INT(i16);
__CONSTRUCT_INT(i32);
__CONSTRUCT_INT(i64);

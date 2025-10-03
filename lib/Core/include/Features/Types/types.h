#pragma once
#define __XC_TYPE__
#include "pkg.h"
#include "utils.h"

typedef uint64_t u64;

typedef uint32_t u32;

typedef uint16_t u16;


typedef uint8_t u8;

typedef int64_t i64;

typedef int32_t i32;


typedef int16_t i16;

typedef int8_t i8;

typedef struct{ char as_cchar; } c8;

typedef char16_t c16;

typedef char32_t c32;

typedef wchar_t  rune;

typedef char* cstr;


typedef wchar_t* wstr;

#undef bool
typedef _Bool bool;

typedef void* pntr;
#define arry(type) type*
#define pntr_shift(ptr, shift_amt) ptr = ((void*)(&(((uint8_t*)ptr)[shift_amt])))
#define pntr_shiftcpy(ptr, shift_amt) ((void*)(&(((uint8_t*)ptr)[shift_amt])))
#define pntr_asVal(addr) (*(uint64_t*)&addr)


typedef void noFail;
typedef u32 errvt;

typedef struct str_t str_t;						
typedef struct strbuff_t strbuff_t;					
typedef u32 FormatID;

#define struct(name, ...) typedef struct name {__VA_ARGS__} name;
#define structDecl(name)  typedef struct name name;

#define enum(name, ...) typedef enum {__VA_ARGS__} name; 			


typedef enum{ 
	#define __ERROR_CODES__
	#include "../../config.h"
	#undef __ERROR_CODES__
}XC_ERROR_CODES;


typedef struct {errvt code; const char* msg} err_t;


typedef struct Object_Instance Object_Instance;
typedef struct Object_ConstructArgs Object_ConstructArgs;
typedef struct Object_Interface Object_Interface;

typedef struct Stream_Instance Stream_Instance;

#define __TYPE_OPERATORS__
#include "../../config.h"
#undef __TYPE_OPERATORS__

#undef OPERATOR
#define OPERATOR(class, name, returnval, ...) returnval (*name)(void* __VA_ARGS__)

struct(TypeOperators, OPERATOR_FUNCS(Object)) 

#undef OPERATOR
#define OPERATOR(class, name, returnval, ...) returnval class##__##name(void* object __VA_ARGS__)

struct(typeData,
     	TypeOperators* ops;
	size_t size;
)
struct(variableData,
	typeData type;
       	void* data;
)


#pragma once
#define __XC_TYPES__
#include "pkg.h"
#include "Classes/pkg.h"

#define package std

typedef struct std_Object std_Object;
typedef struct std_Object_ConstructArgs std_Object_ConstructArgs;
typedef struct std_Object_FormatArgs std_Object_FormatArgs;
typedef struct std_Object_Interface std_Object_Interface;

typedef struct std_Stream std_Stream;



#define fmtArgsOf(type) type##_FormatArgs

#define __TYPE_OPERATORS__
#include "config.c"
#undef __TYPE_OPERATORS__

#undef OPERATOR
#define OPERATOR(class, name, returnval, ...) returnval (*name)(class* __VA_OPT__(, __VA_ARGS__))

typedef struct { OPERATOR_FUNCS(std_Object); } std_typeOperators;

asXCType(std_typeOperators)

typedef struct std_typeData {
     	std_typeOperators* ops;
	len_t size;
	const void* id; 
	const void* format; 
	const void* construct; 
} std_typeData;

asXCType(std_typeData);


type(varData,
	std_typeData type;
       	void* 	     data;
)

#undef package



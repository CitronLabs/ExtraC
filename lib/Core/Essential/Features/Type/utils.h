#pragma once
#define __XC_TYPES__
#include "pkg.h"


#define ctypeDataOf(type)   (std_typeData){null, sizeof(type), NULL, NULL}

#define cvarDataOf(data) (std_varData){(std_typeData){null, sizeof(data), NULL, NULL}, &data}	

#define typeDataOf(type) 				\
	(std_typeData){					\
	    .ops       = generic type##_Type->ops, 	\
	    .size      = type##_Type->size,		\
  	    .id        = generic type##_TypeID,		\
	    .format    = generic type##_Type->format,	\
	    .construct = generic type##_Type->construct }					

#define typeDataFrom(var) 				\
	(std_typeData){					\
	    .ops       = generic (var)->__type->ops, 	\
	    .size      = (var)->__type->size,		\
  	    .id        = generic (var)->__type->id,	\
	    .format    = generic (var)->__type->format,	\
	    .construct = generic (var)->__type->construct }					


#define varDataOf(object)  			\
	(std_varData){ 				\
	    typeDataFrom(object), object}	\
	
#define varDataFrom(typedata, object)  			\
	(std_varData){					\
	    (std_typeData){				\
	    .ops       = generic (typedata)->ops, 	\
	    .size      = (typedata)->size,		\
  	    .id        = generic (typedata)->id,	\
	    .format    = generic (typedata)->format,	\
	    .construct = generic (typedata)->construct }\
	    , generic object}		

#define setAllOps(Class) 				\
		.Write   = generic Class##_Op_Write, 	\
		.Read    = generic Class##_Op_Read, 	\
		.Set     = generic Class##_Op_Set,	\
		.Copy    = generic Class##_Op_Copy,	\
		.Size    = generic Class##_Op_Size,	\
		.Hash    = generic Class##_Op_Hash,	\
		.Destroy = generic Class##_Op_Destroy,	\
		.Iter    = generic Class##_Op_Iter,	\
		.Scan    = generic Class##_Op_Scan,	\
		.Print   = generic Class##_Op_Print,	\

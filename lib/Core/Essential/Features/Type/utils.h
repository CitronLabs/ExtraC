#pragma once
#define __XC_TYPES__
#include "pkg.h"


#define cT(type)   (std_typeData){null, sizeof(type), NULL, NULL}

#define cV(data) (std_varData){(std_typeData){null, sizeof(data), NULL, NULL}, &data}	

#define T(type) 				\
	(std_typeData){					\
	    .ops       = generic type##_Type->ops, 	\
	    .size      = type##_Type->size,		\
  	    .id        = generic type##_TypeID,		\
	    .format    = generic type##_Type->format,	\
	    .construct = generic type##_Type->construct }					

#define TFrom(var) 				\
	(std_typeData){					\
	    .ops       = generic (var)->__type->ops, 	\
	    .size      = (var)->__type->size,		\
  	    .id        = generic (var)->__type->id,	\
	    .format    = generic (var)->__type->format,	\
	    .construct = generic (var)->__type->construct }					


#define V(object)  				\
	(std_varData){ 				\
	    TFrom(object), object}	\
	
#define VFrom(typedata, object)  			\
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


#define Type std_typeData


#define switchT(typeData) 					\
	if(typeData) 						\
	    for(std_TypeID __switch_type_id = (typeData)->id; 	\
	    __switch_type_id; __switch_type_id = null)

#define caseT(type) if(__switch_type_id == typeID(type))

#define defaultT for(std_TypeID ID = __switch_type_id; ID; ID = null)

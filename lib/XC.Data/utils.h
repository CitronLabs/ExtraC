#pragma once

#define __XC_DATA__ 
#include "pkg.h"


/**
@def List(type)
@brief readability macro for being explicit about what the list is intended to store
*/
#define List(type) std_List* 


#define Map(key,value) std_Map*

/**
@{
@def RESERVE_MACROS 
@brief aliases for true and false for use in the **bool exact** parameter in reserve methods
*/
#define RESERVE_EXACT   true
#define RESERVE_ATLEAST false
/**@}*/

/**
@{
@def LISTINDEX_MACROS 
@brief aliases for true and false for use in the **bool write** parameter in the List.Index method
@details as to avoid having to use the double comma when calling the macro this only works with 
more than 1 buffer element

the type is infered using typeof() from the first parameter, then the others are assumed to 
be of the same type
*/
#define LISTINDEX_WRITE true
#define LISTINDEX_READ false
/**@}*/


/**
@def getDSN_Type(var)
@return the DSN_fieldType of var or DSN_NULL if not applicable
*/

#define getDSN_Type(var) _Generic((var),\
std_Number*	: std.DSN.Field.NUMBER,		\
std_String*	: std.DSN.Field.STRING,		\
std_List*	: std.DSN.Field.LIST,		\
std_Map*	: std.DSN.Field.MAP,		\
std_Struct*	: std.DSN.Field.STRUCT,		\
std_Number	: std.DSN.Field.NUMBER,		\
std_String	: std.DSN.Field.STRING,		\
std_List	: std.DSN.Field.LIST,		\
std_Map		: std.DSN.Field.MAP,		\
std_Struct	: std.DSN.Field.STRUCT,		\
default		: std.DSN.Field.NONE)

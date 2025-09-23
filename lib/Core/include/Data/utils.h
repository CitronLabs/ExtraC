#pragma once

#define __XC_DATA__ 
#include "pkg.h"


/**
@def List(type)
@brief readability macro for being explicit about what the list is intended to store
*/
#define List(type) inst(List) 

/**
@def Queue(type)
@brief readability macro for being explicit about what the queue is intended to store
*/
#define Queue(type) inst(Queue) 

/**
@def Stack(type)
@brief readability macro for being explicit about what the stack is intended to store
*/
#define Stack(type) inst(Stack) 

#define Map(key,value) inst(Map)

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

#define getDSN_Type(var) _Generic((var),		\
inst(Number)	: DSN_NUMBER,				\
inst(String)	: DSN_STRING,				\
inst(List)	: DSN_LIST,				\
inst(Queue)	: DSN_QUEUE,				\
inst(Stack)	: DSN_STACK,				\
inst(Map)	: DSN_MAP,				\
inst(Struct)	: DSN_STRUCT,				\
data(Number)	: DSN_NUMBER,				\
data(String)	: DSN_STRING,				\
data(List)	: DSN_LIST,				\
data(Queue)	: DSN_QUEUE,				\
data(Stack)	: DSN_STACK,				\
data(Map)	: DSN_MAP,				\
data(Struct)	: DSN_STRUCT,				\
default		: DSN_NULL)

#pragma once
#include "__macro_utils.h"

typedef struct Module_Sources{
	char
	    *   MODULE_NAME,
	    **  MODULE_SOURCES,
	    **  MODULE_INCLUDE;

	long long MODULE_SOURCES_LEN, MODULE_INCLUDE_LEN;


} Module_Sources;

#define STRINGIFY_IMPL(...) 	#__VA_ARGS__
#define STRINGIFY(...) 		STRINGIFY_IMPL(__VA_ARGS__)

#define docs(name, ...) volatile const char __MODULE_CAT_NAME(name, Docs)[] = PP_STRINGIFY(__VA_ARGS__);

#define include(...) __VA_ARGS__


#define source(include, ...) 								\
	volatile const Module_Sources __MODULE_CAT_NAME(module, Sources) = {		\
		.MODULE_NAME 	    = STRINGIFY(__MODULE_CAT_NAME(module)),		\
		.MODULE_SOURCES     = (char*[]){QUOTE_LIST(__VA_ARGS__)}, 		\
		.MODULE_SOURCES_LEN = 							\
			sizeof((char*[]){QUOTE_LIST(__VA_ARGS__)}) / sizeof(char*),	\
		.MODULE_INCLUDE     = (char*[]){QUOTE_LIST(include)},			\
		.MODULE_INCLUDE_LEN = 							\
			sizeof((char*[]){QUOTE_LIST(include)}) / sizeof(char*),		\
	};

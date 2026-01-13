#pragma once
#include <Env.pkg.h>

#define module env, Common, Devices, Manager

moduleValues(PATH(Resource, Type),
	REGISTER,,
	STREAM,,
)

exportFrom(Resource,
SUBMODULE(),
VALUES(Type),
	add, remove, release, grab
)


export(
SUBMODULE(Resource),
VALUES(),
	getOne, getAll, add, find, 
	remove, isConnected, disconnect, 
	grab, release);


#undef module

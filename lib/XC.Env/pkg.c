#pragma once
#include <XC.pkg.h>

/*--------------------------------------|
 *					|
 *      sys.Device.Register Module	|
 *					|
 *------------------------------------*/
#define module sys, Register

moduleValues(Attrib,
      	READ  		as (1 << 0), 
      	WRITE 		as (1 << 1),
      	DIRECT_ACCESS 	as (1 << 2)
);

export(
SUBMODULE(),
VALUES(Attrib),
	open, fetch, close, delete,

	watch, isModified, 
	info, writeTo, readFrom, setTo, access
);


#undef module
/*--------------------------------------|
 *					|
 *	system.Device.Stream Module	|
 *					|
 *------------------------------------*/
#define module sys, Stream


moduleValues(Attrib,
      	READ  as (1 << 0), 
      	WRITE as (1 << 1)
);

export(
SUBMODULE(),
VALUES(Attrib),
	open, fetch, close, edit, delete,

	watch, isModified, 
	info, writeTo, readFrom, shift, sync, 
	flush, control
);


#undef module
/*--------------------------------------|
 *					|
 *	  system.Device Module		|
 *					|
 *------------------------------------*/
#define module sys, Device

export(
SUBMODULE(),
VALUES(),
	init, close, fetch, info);


#undef module

/*--------------------------------------|
 *					|
 *	      system Module		|
 *					|
 *------------------------------------*/
#define module sys

moduleValues(Version,
	MAJOR, 0,
	MINOR, 1,
	PATCH, 0
);


export(
SUBMODULE(Device, Stream, Register),
VALUES(Version),
);

#undef module

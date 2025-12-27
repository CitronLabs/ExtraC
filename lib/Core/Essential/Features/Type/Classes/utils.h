#pragma once
#define __XC_TYPES__
#include "../pkg.h"

/*======================|
 *   Construct Utils
======================*/


#define setpriv(of) of->__private = typeof(of->__private)
#define setthis this = typeof(this)

#define passover memcpy(self, args, sizeof(*args)); return self;

#define typeID(type) type##_TypeID

/*======================|
 *   	 Utils
======================*/

#define priv self->__private
#define this (*self)
#define arg  (*args)
#define ops(type) (*(type)->ops)
#define self(Class) Class* self = (Class*) object;
#define privof(object) object->__private


#define intf(Class) const Class##_Interface*
#define ifob(interface) interface##_InterfaceObj

#define with(_interface, _object) 				\
	(interface##_InterfaceObj)				\
  		{.interface = _interface, .object = _object}


#define obj(Class) struct { Class public; Class##_Private private; }
#define argsof(Class) Class##_ConstructArgs
#define i(ifob) (*((ifob)->interface))
#define o(ifob) ((ifob)->object)

#define generic (void*)
#define like(var) (typeof(var))


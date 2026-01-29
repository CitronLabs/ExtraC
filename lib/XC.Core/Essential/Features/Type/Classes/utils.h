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


#define intf(Class) const ___(Class, Interface)
#define ifob(interface) ___(interface, Interface_Obj)*

#define with(interface, object) generic &(struct{typeof(interface) a; pntr b;}){interface, object}

#define ifob_with(_interface) 			\
	(___(_interface, Obj))	\


#define argsof(Class) ___(Class, ConstructArgs)
#define i(ifob) (*((ifob)->interface))
#define o(ifob) ((ifob)->object)

#define generic (void*)
#define like(var) (typeof(var))





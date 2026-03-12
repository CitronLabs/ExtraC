#pragma once
#include "utils.h"
#include "types.h"

#include "System/Device.h"
#include "System/Register.h"
#include "System/Stream.h"

#include "Runtime/pkg.h"

typedef struct sys_Interface {
	struct {
	    uword
		MAJOR,
		MINOR,
		PATCH;
	} Version;

	__XC_DEVICE_Interface 	Device;
	__XC_REGISTER_Interface Register;
	__XC_STREAM_Interface   Stream;
}sys_Interface;

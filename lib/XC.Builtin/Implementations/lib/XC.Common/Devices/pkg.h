#pragma once
#define __ENV_COMMON__
#include "../pkg.h"

#ifndef __ENV_COMMON_DEVICES__

#include "types.h"
#include "Manager/pkg.h"

#define package env_Common

Interface(Devices,
	interface(env_Common_Devices_Manager) Manager;
)

#undef package

#endif




#pragma once
#include <XC.Core/pkg.h>
#include <XC.Data/pkg.h>
#include <XC.Alloc/pkg.h>


#ifndef __XC_SYSTEM__

#include "types.h"
#include "Memory/pkg.h"
#include "Shell/pkg.h"

#define package std

Interface(System,

	interface(std_System_Memory) Memory;
	interface(std_System_Shell)  Shell;

	values(Capabilities, u64,
		MODULE_LOADING,
		VIRTUAL_MEMORY,
		MULTI_THREADING,
		SHELL_ACCESS
	)

	values(SettingID, len_t,
		SYSTEM_NAME,
		SYSTEM_VERSION,
		IMPL_NAME,
		IMPL_VERSION,
		CAPS_BITFIELD
	)

  submodule(EnvVar,
	const c8* 	fn(getData)(const c8* name);
	errvt 		fn(setData)(const c8* name, strc8 value);
	errvt 		fn(addData)(const c8* name, strc8 value);
	std_Array_List	fn(listData)(const c8* name);
	errvt 		fn(delete)(const c8* name);
	errvt 		fn(init)(const c8* name);
  )

	std_System_Info fn(getSystemInfo)();

)

#undef package
#endif

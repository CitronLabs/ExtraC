#pragma once
#include <XC.pkg.h>
#define module std, System



moduleValues(Capabilities,
	MODULE_LOADING	as (1 << 0),
	VIRTUAL_MEMORY	as (1 << 1),
	MULTI_THREADING as (1 << 2),
	SHELL_ACCESS	as (1 << 3)
)

moduleValues(SettingID,
	SYSTEM_NAME,,
	SYSTEM_VERSION,,
	IMPL_NAME,,
	IMPL_VERSION,,
	CAPS_BITFIELD,,
)

exportFrom(EnvVar,
SUBMODULE(),
VALUES(),
	getData, setData, addData, 
	listData, delete, init
)

export(
SUBMODULE(EnvVar),
VALUES(Capabilities, SettingID),
	getSystemInfo
)

#undef module

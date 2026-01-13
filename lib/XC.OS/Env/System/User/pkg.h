#pragma once
#define __XC_OS_ENV_SYSTEM__
#include "../pkg.h"


#ifndef __XC_OS_ENV_SYSTEM_USER__

#include "types.h"

#define package os_Env

Interface(User, 
	const strc8 stdVersion;
	values(perms, UserPermissions,
		READ, 	    
		WRITE, 	    
		EXECUTE, 	    
		DELETE, 	    
		LIST_CONTENTS,
		OWNERSHIP,    
		CREATE_CHILD, 
		DELETE_CHILD;
	)

	UserInfo* 	fn(enumUsers)		(u32* count);
	UserHandle 	fn(fetchUser)		(UserInfo* info);
	UserHandle 	fn(fetchFromName)	(strc8 name);
	errvt 		fn(closeUser)		(UserHandle handle);
	errvt 		fn(removeUser)		(UserHandle handle);
	errvt 		fn(setPermssions)	(UserHandle handle, UserPermissions perms);
	errvt 		fn(setPassword)		(UserHandle handle, strc8 password);
	errvt 		fn(logoutUser)		(UserHandle handle);
	UserHandle 	fn(loginUser)		(strc8 Username, strc8 password);
	bool 		fn(isSubUser)		(UserHandle User, UserHandle parent);
	bool 		fn(isAdmin)		(UserHandle User);
	errvt 		fn(getInfo)		(UserHandle User, UserInfo* info);
	errvt 		fn(getCurrent)		(UserInfo* info);
	errvt 		fn(getUserPath)		(UserHandle User, strc8 path);
)

#undef package
#undef UserInfo
#undef UserHandle
#undef UserPermissions
#endif

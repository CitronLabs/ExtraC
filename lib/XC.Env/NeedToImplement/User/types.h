#pragma once
#define __XC_OS_SYSTEM_USER__
#include "pkg.h"

#define package os_Sys_User

#define UserPermissions os_Sys_User_Permissions
#define UserHandle 	os_Sys_User_Handle
#define UserInfo 	os_Sys_User_Info

// encompasses both groups and individual users
typefrom(pntr, Handle)
typefrom(word, Permissions)

type(Info,
	std_String 	  userName;
	std_String 	  fullName;
    	Array(std_String) groups;
    	Array(std_String) subUsers;
    	UserPermissions   perms;
)

#undef package

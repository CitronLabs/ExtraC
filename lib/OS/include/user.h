#pragma once
#include "./extern.h"

// encompasses both groups and individual users
typedef void* userHandle;

typedef int userPermissions;

type(userInfo,
	inst(String) userName;
	inst(String) fullName;
    	Buffer(data(String)) groups;
    	Buffer(data(String)) subUsers;
    	userPermissions perms;
)


Interface(user, 
	const strc8 stdVersion;
	submodule(perms,
		userPermissions
		READ, 	    
		WRITE, 	    
		EXECUTE, 	    
		DELETE, 	    
		LIST_CONTENTS,
		OWNERSHIP,    
		CREATE_CHILD, 
		DELETE_CHILD;
	)
	userInfo* 	vmethod(enumUsers,   	u32* count);
	userHandle 	vmethod(fetchUser,   	userInfo* info);
	userHandle 	vmethod(fetchFromName, 	strc8 name);
	errvt 		vmethod(closeUser,	userHandle handle);
	errvt 		vmethod(removeUser,	userHandle handle);
	errvt 		vmethod(setPermssions,	userHandle handle, userPermissions perms);
	errvt 		vmethod(setPassword, 	userHandle handle, strc8 password);
	errvt 		vmethod(logoutUser,	userHandle handle);
	userHandle 	vmethod(loginUser,    	strc8 username, strc8 password);
	bool 		vmethod(isSubUser,   	userHandle user, userHandle parent);
	bool 		vmethod(isAdmin,     	userHandle user);
	errvt 		vmethod(getInfo,     	userHandle user, userInfo* info);
	errvt 		vmethod(getCurrent,    	userInfo* info);
	errvt 		vmethod(getUserPath,    userHandle user, strc8 path)
)

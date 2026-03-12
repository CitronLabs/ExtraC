#include <Env.pkg.c>
#include <XC.pkg.c>
#define module std, System


std_System_Info moduleFn(getSystemInfo)(){
	var SystemDevice  = env.Devices.XC.Sys.getHandle(nil);

	var infoReg = sys.Register.fetch(SystemDevice, "Info");

	if(!infoReg){
	    ERR(ERR.FAIL, "Failed to fetch system info register");
	    return (std_System_Info){0};
	}

	std_System_Info info = {};

	if(!sys.Register.Modify.readFrom(infoReg, &info, 0)){
	    ERR(ERR.FAIL, "Failed to read system info register");
	    return (std_System_Info){0};
	}

return info;
}





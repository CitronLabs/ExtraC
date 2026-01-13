#include <XC/pkg.c>

#define module XC, Dev

from(env_Common,
	Devices_ID as devID
)

devHandle moduleFn(open)(const char* name, word attributes){
ERR(ERR.NOTIMPLEM, 
	"This feature is not currently avalible in "
	"this version of the Linux XC Implementation"
);

return nil;
}
devHandle moduleFn(fetch)(const char* name){
ERR(ERR.NOTIMPLEM, 
	"This feature is not currently avalible in "
	"this version of the Linux XC Implementation"
);

return nil;
}

devHandle moduleFn(stdHandle)(word id){

	switch(id){
	case XC_Dev_ID_IO:{
		return (devHandle)(pntrval)env.Linux.Runtime.Devices.getIO();
	break;}
	case XC_Dev_ID_Sys:{
		return (devHandle)(pntrval)env.Linux.Runtime.Devices.getSys();
	break;}
	default: {
		ERR(ERR.INVALID, "invalid std device handle id");
		return nil;
	}
	}
}


#include <XC.pkg.c>
#include <Env.pkg.h>

#define module env, Windows, Runtime, Device

from(env_Common_Devices,
     	use(Manager),
	ID 		as devID,
     	ResourceID 	as rsrcID,
	Entry 		as devEntry,
	ResourceInfo	as rsrcInfo,
	Resource	as rsrcEntry
)

alias(env.Windows.Runtime.Device,  WinRTDev)
alias(env.Windows.Runtime.Console, WinRTCon)
alias(env.Windows.Utils, 	   WinUtils)
alias(env.Common.Devices.Manager,  Dev)
alias(core.Device.Attrib, 		   DevAttrib)


static const deviceInfo 

IODeviceInfo = {
	.name 		= "Windows_XC_IO",
	.vendorName 	= "CitronLabs",
	.productName  	= "Windows_XC_IO_Interface",
	.path		= "XC.IO",
	.attributes 	= DevAttrib.PRIVATE 		| 
			  DevAttrib.STREAM_CREATE 	| // is later set to STREAM_CREATE only
			  DevAttrib.REGISTER_CREATE,
	.valid 		= true
},
SysDeviceInfo = {
	.name 		= "Windows_XC_Sys",
    	.vendorName 	= "CitronLabs",
    	.productName  	= "Windows_XC_Sys_Interface",
    	.path		= "XC.Sys",
    	.attributes 	= DevAttrib.PRIVATE 		| 
			  DevAttrib.STREAM_CREATE 	| // is later set to REGISTER_CREATE only
			  DevAttrib.REGISTER_CREATE,
    	.valid 		= true
};

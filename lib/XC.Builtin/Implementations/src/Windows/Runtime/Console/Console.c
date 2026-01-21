#include "Console.h"

static bool isInit = false;


HANDLE moduleFn(getStream)(word id){
	if(!isInit) mod(init)();

	DWORD handleType = 
		id == WinRTCon.StreamType.INPUT  ? STD_INPUT_HANDLE :
		id == WinRTCon.StreamType.OUTPUT ? STD_OUTPUT_HANDLE :
		id == WinRTCon.StreamType.ERR    ? STD_ERROR_HANDLE :
		-1;

	if(handleType == -1){
		ERR(ERR.INVALID, "Invalid stream type ID");
		return nil;
	}

return GetStdHandle(STD_INPUT_HANDLE);
}
void moduleFn(getInfo)(word){


}

void moduleFn(init)(){

	if(!AttachConsole(ATTACH_PARENT_PROCESS)){
		ERR(ERR.INIT, "Failed to attach console to this process");
		return;
	}

	isInit = true;
}

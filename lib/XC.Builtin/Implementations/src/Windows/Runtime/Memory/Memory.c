#include "Memory.h"

static XC_Memory_Info MemInfo = {};

typedef errvt(*FaultHandler)(void*, len_t);


LONG moduleFn(FaultHandler)(struct _EXCEPTION_POINTERS* info){

	switch(info->ExceptionRecord->ExceptionCode){
	case EXCEPTION_ACCESS_VIOLATION:{
		pntr addr = info->ExceptionRecord->ExceptionAddress;
		
		if(addr >= nil || addr <= nil + MemInfo.pageSize){
			// ERR.NIL
		}
		

	
	return EXCEPTION_CONTINUE_SEARCH;}
	default:{ return EXCEPTION_CONTINUE_SEARCH; }
	}
}

void moduleFn()(){


}

void moduleFn(init)(){

	AddVectoredContinueHandler(
		true,
		mod(FaultHandler)
	);

}

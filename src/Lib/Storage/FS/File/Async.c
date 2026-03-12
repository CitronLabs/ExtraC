#pragma once
#include "../FS.h"
#define module std, FS, File, Async

typedef const struct std_File_Async_Proc 	Proc;

void NoOp();

static Proc 
CONTINUE = {
	.start		=	mod(start),  
	.end		=	mod(end),  
	.run		=	mod(run),  
	.fail		=	mod(fail),  
	.result		=	mod(result), 
	.cpy		= 	mod(cpy), 
	.move		= 	mod(move), 
	.rename		= 	mod(rename), 
	.delete		= 	mod(delete), 
	.createAt	= 	mod(createAt),
	.writeData	= 	mod(writeData), 
	.readData	= 	mod(readData), 
	.writeFmt	= 	mod(writeFmt), 
	.readFmt	= 	mod(readFmt),
	.waitFor	= 	mod(waitFor), 
	.isDone         =       mod(isDone)
};

static Proc 
STOP = {
	.start		=	generic NoOp,  
	.end		=	generic NoOp,  
	.run		=	generic NoOp,  
	.fail		=	generic NoOp,  
	.result		=	generic NoOp, 
	.cpy		= 	generic NoOp,
	.move		= 	generic NoOp,
	.rename		= 	generic NoOp,
	.delete		= 	generic NoOp,
	.createAt	= 	generic NoOp,
	.writeData	= 	generic NoOp,
	.readData	= 	generic NoOp,
	.writeFmt	= 	generic NoOp,
	.readFmt	= 	generic NoOp,
	.waitFor	= 	generic NoOp,
	.isDone         =	generic NoOp        
};


thread_local struct {
	Stack files;
	std_File* current;

} Process;

Stack* moduleFn(getFilesStack)(){
	
	if(!Process.files.__type){
	    if(!createArrayStack(std_File*, &Process.files, 5)){
		ERR(ERR.INIT, "Failed to initialize string buffers stack");
		return nil;
	    }
	}
return &Process.files;
}

Proc moduleFn(start)(std_File* self){
	nonull(self) return STOP;

	var fileStack = mod(getFilesStack)();

	if(Process.current)
		write(fileStack, &Process.current);

	Process.current = self;

return CONTINUE;
}

noFail moduleFn(end)(){
	var fileStack = mod(getFilesStack)();
	
	if(len(fileStack) != 0)
		read(fileStack, &Process.current);
	
}
noFail moduleFn(fail)(){ mod(end)(); }

std_File* moduleFn(result)(){ return Process.current; }

Proc moduleFn(run)(bool cont){ return cont ? CONTINUE : STOP; }

Proc moduleFn(cpy)(std_FS_Path newPath){



}
Proc moduleFn(move)(std_FS_Path newPath){}
Proc moduleFn(rename)(std_FS_Path newName){}
Proc moduleFn(delete)(std_FS_Path atPath){}
Proc moduleFn(createAt)(std_FS_Path atPath){}
Proc moduleFn(writeData)(void* data, len_t len){}
Proc moduleFn(readData)(void* data, len_t len){}
Proc moduleFn(writeFmt)(std_Types_Format_Args args){}
Proc moduleFn(readFmt)(std_Types_Format_Args args){}
Proc moduleFn(waitFor)(){}

bool moduleFn(isDone)(){}

#undef module

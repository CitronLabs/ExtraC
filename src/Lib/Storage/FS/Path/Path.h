#pragma once
#include "../FS.h"
#define module std, FS, Path

typedef const struct std_FS_Path_Path_Proc Proc;

void NoOp();


static Proc 
CONTINUE = {
	.start		=	mod(start),  
	.end		=	mod(end),  
	.run		=	mod(run),  
	.fail		=	mod(fail),  
	.result		=	mod(result), 
	.join		=	mod(join),
	.setExtension	=	mod(setExtension),
	.rename		=	mod(rename),
	.move		=	mod(move),		
	.chdir		=	mod(chdir),		
	.getFileName	=	mod(getFileName),
	.getExtension	=	mod(getExtension),	
	.isAbsolute	=	mod(isAbsolute),
	.isRelative	=	mod(isRelative),
	.hasExtension	=	mod(hasExtension),
	.isChildOf	=	mod(isChildOf)
};

static Proc 
STOP = {
	.start		=	generic NoOp,  
	.end		=	generic NoOp,  
	.run		=	generic NoOp,  
	.fail		=	generic NoOp,  
	.result		=	generic NoOp, 
	.join		=	generic NoOp,
	.setExtension	=	generic NoOp,
	.rename		=	generic NoOp,
	.move		=	generic NoOp,
	.chdir		=	generic NoOp,
	.getFileName	=	generic NoOp,
	.getExtension	=	generic NoOp,
	.isAbsolute	=	generic NoOp,
	.isRelative	=	generic NoOp,
	.hasExtension	=	generic NoOp,
	.isChildOf	=	generic NoOp
};

#undef module

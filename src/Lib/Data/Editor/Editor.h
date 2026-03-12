#pragma once
#include <XC.pkg.c>

typedef const struct std_String_String_Proc 	StrProc;
typedef const struct pntr_Bytes_Proc 		ByteProc;
typedef const struct std_Array_Array_Proc 	ArrayProc;

from(std,
     	use(List),
	Array_Stack as Stack,

)

alias(std.String.Encoding.UTF32, UTF32);
alias(std.String.Encoding.UTF8,  UTF8);

void NoOp();
#ifdef IMPL_STRING
#define module std, Editor, String
static StrProc 
CONTINUE = {
	.start		=	mod(start),  
	.end		=	mod(end),  
	.run		=	mod(run),  
	.fail		=	mod(fail),  
	.result		=	mod(result), 
	.clear		=	mod(clear),  
	.strip		=	mod(strip),  
	.upper		=	mod(upper),  
	.lower		=	mod(lower),  
	.reverse	=	mod(reverse), 
	.padLeft	=	mod(padLeft),  
	.append		=	mod(append),  
	.prepend	=	mod(prepend),  
	.insert		=	mod(insert),  
	.slice		=	mod(slice),  
	.replace	=	mod(replace),  
	.each		=	mod(each),  
	.getStr		=	mod(getStr),  
	.makeStr	=	mod(makeStr), 
	.find		=	mod(find),  
	.contains	=	mod(contains),  
	.starts		=	mod(starts),  
	.ends		=	mod(ends)
};

static StrProc 
STOP = {
	.start		=	generic NoOp,  
	.end		=	generic NoOp,  
	.run		=	generic NoOp,  
	.fail		=	generic NoOp,  
	.result		=	generic NoOp, 
	.clear		=	generic NoOp,  
	.strip		=	generic NoOp,  
	.upper		=	generic NoOp,  
	.lower		=	generic NoOp,  
	.reverse	=	generic NoOp, 
	.padLeft	=	generic NoOp,  
	.append		=	generic NoOp,  
	.prepend	=	generic NoOp,  
	.insert		=	generic NoOp,  
	.slice		=	generic NoOp,  
	.replace	=	generic NoOp,  
	.each		=	generic NoOp,  
	.getStr		=	generic NoOp,  
	.makeStr	=	generic NoOp, 
	.find		=	generic NoOp,  
	.contains	=	generic NoOp,  
	.starts		=	generic NoOp,  
	.ends		=	generic NoOp
};
#endif

#ifdef IMPL_ARRAY
#define module std, Editor, Array
static ArrayProc 
CONTINUE = {
	.start		=	mod(start),
	.end		=	mod(end),
	.run		=	mod(run),
	.fail		=	mod(fail),
	.result		=	mod(result),
	.push		=	mod(push),
	.pop		=	mod(pop),
	.insert		=	mod(insert),
	.remove		=	mod(remove),
	.reverse	=	mod(reverse),
	.sort		=	mod(sort),
	.each		=	mod(each),
	.filter		=	mod(filter),
	.makeBuffer	=	mod(makeBuffer),
	.makeStack	=	mod(makeStack),
	.makeQueue	=	mod(makeQueue),
	.makeList	=	mod(makeList),
	.grab		=	mod(grab),
	.search		=	mod(search),
	.last		=	mod(last),
	.first		=	mod(first),
	.middle		=	mod(middle)
};

static ArrayProc 
STOP = {
	.start		=	generic NoOp,  
	.end		=	generic NoOp,  
	.run		=	generic NoOp,  
	.fail		=	generic NoOp,  
	.result		=	generic NoOp, 
	.push		=	generic NoOp,  
	.pop		=	generic NoOp,  
	.insert		=	generic NoOp,  
	.remove		=	generic NoOp,  
	.reverse	=	generic NoOp, 
	.sort		=	generic NoOp,  
	.each		=	generic NoOp,  
	.filter		=	generic NoOp,  
	.makeBuffer	=	generic NoOp,  
	.makeStack	=	generic NoOp, 
	.makeQueue	=	generic NoOp,  
	.makeList	=	generic NoOp,  
	.grab		=	generic NoOp,  
	.search		=	generic NoOp,  
	.last		=	generic NoOp,  
	.first		=	generic NoOp,  
	.middle		=	generic NoOp
};
#endif

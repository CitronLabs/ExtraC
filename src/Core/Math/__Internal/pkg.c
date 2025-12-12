#pragma once
#include "pkg.h"

#define module Internal

exportModule(
SUBMODULE(),
VALUES(),
	Copy,	
	clearLeadingZeros,
	absoluteCompare,		
	absoluteAdd,		
	absoluteSub,		
	multiplyByDigit,
	absoluteMultiply,	
	shiftDigitsRight,	
	divide_smallInt,
	IntScan,	
	IntPrintDeci,	
	IntPrintHex,		
	IntPrintBin		
);

#undef module

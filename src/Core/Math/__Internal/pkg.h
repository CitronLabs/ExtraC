#include <Core/pkg.h>
#include "utils.h"

#define package std_Number


Module(Internal){
	noFail fn(clearLeadingZeros)(std_Number* self);

	std_Equality fn(absoluteCompare)(std_Number* a, std_Number* b);

	errvt fn(absoluteAdd)(std_Number* result, std_Number* a, std_Number* b);

	errvt fn(absoluteSub)(std_Number* result, std_Number* a, std_Number* b);

	errvt fn(multiplyByDigit)(std_Number* result, std_Number* self, u32 digit);

	errvt fn(absoluteMultiply)(std_Number* result, std_Number* a, std_Number* b);

	errvt fn(shiftDigitsRight)(std_Number* result, std_Number* self, int shift_blocks);

	errvt fn(Copy)(std_Number* dest, std_Number* src);

	u32 fn(divide_smallInt)(std_Number* self, u32 divisor);

	u64 fn(IntPrintDeci)(std_Number* self, std_Stream* out); 

	u64 fn(IntPrintHex)(std_Number* self, std_Stream* out);

	u64 fn(IntPrintBin)(std_Number* self, std_Stream* out);

	u64 fn(IntScan)(std_Number* self, std_Number_FormatArgs* format, std_Stream* out);
	
};

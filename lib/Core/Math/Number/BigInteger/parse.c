#pragma once
#include "../../../pkg.h"
#include "../utils.h"

import(std)


#ifndef HIDE_USE
use(std,
    Number,
    Stream
)
#endif

u64 methodimpl(std_Number, IntScan, Number_FormatArgs* format, std_Stream* in) {

	std_Number_setZero(self); // Initialize 'self' to zero before parsing
	
	int start_idx = 0; rune c = 0; len_t prev_pos = size(in);
	
	int base_val = format ? format->base : 10;
	
	
	// Clear the initial [0] digit added by initialization
	std.List.Flush(priv.digits);
	
	std.Stream.Process
	.start(in)
	.doDecode(std.String.UTF8.Decoder, c){

	    unsigned int digit_val;
	    
	    switch(c){
	    case '0' ... '9' :{ digit_val = c - '0'; break; }
	    case 'a' ... 'f' :{ digit_val = c - '0'; break; }
	    case 'A' ... 'F' :{ digit_val = c - '0'; break; }
	    default: {
	    	ERR(ERR_INVALID, "Invalid character");
	    	std_Number_setZero(self); // Reset to zero on error
	    	process->fail();
	    	return 0;
	    }
	    }
	    
	    // Validate digit against the base
	    if (digit_val >= base_val) {
	    	ERR(DATAERR_OUTOFRANGE, "Digit out of range");
	    	std_Number_setZero(self); // Reset to zero on error
	    	process->fail();
	    	return 0;
	    }
	    
	    u32 carry = digit_val;

	    foreach(priv.digits, u32, digit){
	    	u64 temp = *digit * base_val + carry;
	    	*digit = (u32)(temp % BIGINT_BASE);
	    	carry = temp / BIGINT_BASE;
	    }
	    
	    if (carry > 0) 
			std.List.Append(priv.digits, &carry, 1); 
	}
	then.end()
	;

	std_Number_clearLeadingZeros(self);

return prev_pos - size(in);
}

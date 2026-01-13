#pragma once
#include <XC.Core/pkg.c>
#include "__Internal/pkg.c"


errvt moduleMethod(std_Number,setZero) {
	nonull(self) { return err; }

	std.List.Flush(priv.digits);
	std.List.Append(priv.digits, &(u64){0}, 1);
	priv.sign = 0;
	if(priv.floating) priv.exponent = 0;

return OK;
}

noFail moduleFn(clearLeadingZeros)(std_Number* self) {
	
	u64* digits = std.List.GetPointer(priv.digits, 0);
	u64 size    = std.List.Size(priv.digits);

	while (size > 1 && digits[size - 1] == 0) {
        	size--;
		std.List.Pop(priv.digits, 1);
		if(priv.floating) priv.exponent--;
    	}
	
	if (size == 1 && digits[0] == 0) {
        	priv.sign = 0;
		if(priv.floating) priv.exponent = 0;
    	}
}

std_Equality moduleFn(absoluteCompare)(std_Number* a, std_Number* b) {
	
	u64 
	    * a_digits = std.List.GetPointer(apriv.digits, 0),
	    * b_digits = std.List.GetPointer(bpriv.digits, 0),
		a_size = std.List.Size(apriv.digits),
		b_size = std.List.Size(bpriv.digits);

    // Compare based on the number of active digits first
    	if (a_size > b_size) return std.Number.Equality.GREATER;
    	if (a_size < b_size) return std.Number.Equality.LESSER;

    // If sizes are equal, compare digit by digit from the most significant (highest index)
	for (int i = a_size - 1; i >= 0; i--) {
    		if (a_digits[i] > b_digits[i]) return std.Number.Equality.GREATER;
    		if (a_digits[i] < b_digits[i]) return std.Number.Equality.LESSER;
    	}
    	return std.Number.Equality.EQUALS; 
}

errvt moduleFn(absoluteAdd)(std_Number* result, std_Number* a, std_Number* b) {
    	
	std.Number.setZero(result); 

	rpriv.sign = 1;   

	u64 
	    * a_digits = std.List.GetPointer(apriv.digits, 0),
	    * b_digits = std.List.GetPointer(bpriv.digits, 0),
		a_size = std.List.Size(apriv.digits),
		b_size = std.List.Size(bpriv.digits);
		
	u32 max_size = (a_size > b_size) ? a_size : b_size,
	    carry = 0; 
	
	
	for (int i = 0; i < max_size || carry; i++) {
	    if (i >= apriv.precision) {
	        std.Number.setZero(result); // Reset result on overflow
	        return ERR(ERR.DATA.OUTOFRANGE, "number overflows");
	    }
	
	    u64 sum = (u64)carry;
	    if (i < a_size) sum += a_digits[i];
	    if (i < b_size) sum += b_digits[i];
	
	    std.List.Append(rpriv.digits, &(u64){sum % BIGINT_BASE}, 1); 	// Store the current digit
	    carry = sum / BIGINT_BASE;                     			// Calculate the carry for the next digit
	}
	
	Internal.clearLeadingZeros(result); 
return OK;
}

errvt moduleFn(absoluteSub)(std_Number* result, std_Number* a, std_Number* b) {
	// ensure abs(a) is greater than or equal to abs(b)
	if (Internal.absoluteCompare(a, b) < 0) {
	    std.Number.setZero(result); 
	    return ERR(ERR.INVALID, "Cannot subtract larger absolute value from smaller absolute value");
	}
	
	std.Number.setZero(result); 
	rpriv.sign = 1;    
	    u32 * b_digits = std.List.GetPointer(bpriv.digits, 0),
	    	b_size = std.List.Size(bpriv.digits);
	
	int borrow = 0;
	foreach(apriv.digits, u32, digit) {
	    u64 diff = *digit - borrow;
	    if (digit_iterator < b_size) {
	        diff -= b_digits[digit_iterator];
	    }
	
	    if (diff < 0) {
	        diff += BIGINT_BASE; // Add the base to make the difference positive
	        borrow = 1;          // Set borrow for the next digit
	    }
	    else {
	        borrow = 0;          // No borrow needed
	    }
	    std.List.Append(rpriv.digits, &diff, 1);
	}
	
	Internal.clearLeadingZeros(result); 
return OK;
}
errvt moduleFn(multiplyByDigit)(std_Number* result, std_Number* self, u32 digit) {
	std.Number.setZero(result);
	if (digit == 0 || priv.sign == 0) return OK; // Result is zero
	
	
	u32 * digits = std.List.GetPointer(priv.digits, 0),
	size = std.List.Size(priv.digits),
	carry = 0;

	loop(i, size || carry) {
	    if (i >= priv.precision) {
	        std.Number.setZero(result); // Reset result on overflow
	        return ERR(ERR.DATA.OUTOFRANGE, "number overflows");
	    }
	    u64 product = carry;
	    if (i < size) {
	        product += (u64)digits[i] * digit;
	    }
	    std.List.Append(rpriv.digits, &(u32){product % BIGINT_BASE}, 1);
	    carry = product / BIGINT_BASE;
	}
	Internal.clearLeadingZeros(result);
return OK;
}

errvt moduleFn(absoluteMultiply)(std_Number* result, std_Number* a, std_Number* b){
	u32 
	    * a_digits = std.List.GetPointer(apriv.digits, 0),
	    * b_digits = std.List.GetPointer(bpriv.digits, 0),
	    * r_digits = std.List.GetPointer(rpriv.digits, 0),
		a_size = std.List.Size(apriv.digits),
		b_size = std.List.Size(bpriv.digits),
		r_size = std.List.Size(rpriv.digits);
	
	// Perform grade-school multiplication
	loop(i, a_size) {
	    u64 carry = 0;
	    loop(j, b_size || carry) {
	        // Calculate the product of current digits plus carry and existing digit in result
	        u64 product = (u64)r_digits[i + j] + carry;
	        if (j < b_size) {
	            product += (u64)a_digits[i] * b_digits[j];
	        }
	
	        // Store the current digit and calculate new carry
	        r_digits[i + j] = product % BIGINT_BASE;
	        carry = product / BIGINT_BASE;
	
	        // Update result size if a new highest digit is set
	        if (i + j + 1 > r_size) {
	            r_size = i + j + 1;
	        }
	    }
	    // If there's a final carry from this row, add it to the next position
	    if (carry > 0) {
	        if (i + b_size >= rpriv.precision) {
			return ERR(ERR.DATA.OUTOFRANGE, "number overflows");
	        }
	        r_digits[i + b_size] += carry;
	        if (i + b_size + 1 > r_size) {
	             r_size = i + b_size + 1;
	        }
	    }
	}
return OK;
}

errvt moduleFn(shiftDigitsRight)(std_Number* result, std_Number* self, int shift_blocks) {
	std.Number.setZero(result);
	if (priv.sign == 0) return OK;
	
	u64 
	    * digits = std.List.GetPointer(priv.digits, 0),
		size = std.List.Size(priv.digits);
	
	if (size + shift_blocks > priv.precision) {
	    std.Number.setZero(result);
	    return ERR(ERR.DATA.OUTOFRANGE, "number overflows");
	}
	
	rpriv.sign = priv.sign;
	std_List* res_digits = std.List.SubList(priv.digits, 0, size);

	std.List.Insert(res_digits, (u64)shift_blocks, size - shift_blocks, &digits[shift_blocks]);
	
	u64* res_digitsptr = std.List.GetPointer(res_digits, 0);
	loop(i, shift_blocks)
	    res_digitsptr[i] = 0;
	
	Internal.clearLeadingZeros(result);
return OK;
}


errvt moduleFn(Copy)(std_Number* dest, std_Number* src) {

	nonull(dest, src){ return err; }

	dest->__private.digits = std.List.SubList(src->__private.digits, 0, maxof(u64));
	dest->__private.sign = src->__private.sign;

return OK;
}

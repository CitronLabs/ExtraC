#pragma once
#include "../../../pkg.h"
#include "../utils.h"

import(std)


errvt moduleMethod(std_Number, IntAdd, std_Number* other, std_Number* result) {

	try() {
	    if (priv.sign == opriv.sign) {
	       	std_Number_absoluteAdd(result, self, other);
	    		rpriv.sign = priv.sign;
	    } else {
	    	switch(std_Number_absoluteCompare(self, other)) { 
	    	case NUM_GREATER:{
	    		std_Number_absoluteSub(result, self, other); 
	    		rpriv.sign = priv.sign;
	    	break;}
	    	case NUM_LESSER:{ // abs(self) < abs(other)
	    		std_Number_absoluteSub(result, other, self);
	    		rpriv.sign = opriv.sign;
	    	break;}
	    	case NUM_EQUALS:{
	    		std_Number_setZero(result);
	    	break;}
	    	default:{ ERR(ERR_NULLPTR, "invalid input"); }
	    	}
	    }
	} catch {
		del(result);
		return err->errorcode;
	}


	std_Number_clearLeadingZeros(result);

return OK;
}

 errvt moduleMethod(std_Number, IntSubtract, std_Number* other, std_Number* result) {
	
	try() {
	    if (priv.sign != opriv.sign) {
	       	std_Number_absoluteAdd(result, self, other);
	    		rpriv.sign = priv.sign;
	    } else {
	    	switch(std_Number_absoluteCompare(self, other)) { 
	    	case NUM_GREATER:{
	    		std_Number_absoluteSub(result, self, other); 
	    		rpriv.sign = priv.sign;
	    	break;}
	    	case NUM_LESSER:{ // abs(self) < abs(other)
	    		std_Number_absoluteSub(result, other, self);
	    		rpriv.sign = -(priv.sign);
	    	break;}
	    	case NUM_EQUALS:{
	    		std_Number_setZero(result);
	    	break;}
	    	default:{ERR(ERR_NULLPTR, "invalid input");}
	    	}
	    }
	} catch {
		del(result);
		return err->errorcode;
	}

	std_Number_clearLeadingZeros(result);

return OK;
}

errvt moduleMethod(std_Number, IntMultiply, std_Number* other, std_Number* result) {
	

	rpriv.sign = (priv.sign == opriv.sign) ? 1 : -1;
	
	rpriv.precision = priv.precision > opriv.precision ? priv.precision : opriv.precision;
	
	iferr(std_Number_absoluteMultiply(result, self, other)){
		return err;
	}

	std_Number_clearLeadingZeros(result); // Clean up leading zeros
return OK;
}

#define rmpriv (remainder->__private)

errvt moduleMethod(std_Number, IntDivide, std_Number* other, std_Number* remainder, std_Number* result) {

    
	std_Number_setZero(remainder);

	errvt reserr = OK;
	u64 current_remainder_val = 0, q_hat = 0, 
		precision = priv.precision > opriv.precision ? priv.precision : opriv.precision;
	u32 
		* a_digits = std.List.GetPointer(priv.digits, 0), * b_digits = std.List.GetPointer(opriv.digits, 0),
		a_size = std.List.Size(priv.digits), b_size = std.List.Size(opriv.digits);
	
	iferr(std.List.Reserve(rpriv.digits, RESERVE_EXACT, precision)){
		reserr = err; goto exit;
	}
	u32* resbuff = std.List.GetPointer(rpriv.digits, 0);
	
	std_Number 
		* tempRemainder = makeTempNum(pushList(u32, 10),   precision), 
		* tempProduct	= makeTempNum(pushList(u32, 10),   precision);
	
	for (int i = a_size - 1; i >= 0; i--) {
		// Shift the current remainder left by BIGINT_BASE and add the next digit from the dividend
		current_remainder_val = current_remainder_val * BIGINT_BASE + a_digits[i];
	
		if (current_remainder_val > b_digits[b_size - 1]) {
			q_hat = current_remainder_val / b_digits[b_size - 1];
			if (q_hat >= BIGINT_BASE) q_hat = BIGINT_BASE - 1;
		}
		
		iferr(std.List.Append(tempRemainder->__private.digits, &current_remainder_val, 2)){
			reserr = err; goto exit;
		}

			std_Number_clearLeadingZeros(tempRemainder);



			iferr(std_Number_multiplyByDigit(tempProduct, other, q_hat)){
			reserr = err; goto exit;
		}
			while (std_Number_absoluteCompare(tempProduct, tempRemainder) == NUM_GREATER) {
				q_hat--;
				iferr(std_Number_multiplyByDigit(tempProduct, other, q_hat)){
				reserr = err; goto exit;
			}
			}
	
			iferr(std_Number_absoluteSub(tempRemainder, tempRemainder, tempProduct)){
			reserr = err; goto exit;
		}

			resbuff[i] = q_hat;
		std.List.Flush(tempRemainder->__private.digits);
		std.List.Flush(tempProduct->__private.digits);
	}
	memcpy(&current_remainder_val, 
		std.List.GetPointer(tempRemainder->__private.digits, 0), 
		std.List.Size(tempRemainder->__private.digits)
	);

	// The final `current_remainder_val` is the remainder.
	std_Number_setZero(remainder);
	rmpriv.sign = priv.sign;
	iferr(std.List.Append(rmpriv.digits, &current_remainder_val, 2)){ reserr = err; goto exit; }
	std_Number_clearLeadingZeros(remainder);

	std_Number_clearLeadingZeros(result);

	// Apply the correct sign to the result
	rpriv.sign = (priv.sign == opriv.sign) ? 1 : -1;
	if (isZero(result)) { // Ensure result is 0 if it became 0
		rpriv.sign = 0;
	}
exit:	
	pop(tempProduct->__private.digits);
	pop(tempRemainder->__private.digits);
	
return reserr;
}

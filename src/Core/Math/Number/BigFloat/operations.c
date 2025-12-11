#pragma once
#include "../utils.h"


errvt moduleMethod(std_Number, SetPrecision, u64 new_precision) {
	nonull(self){ return err; }
	
	priv.precision = new_precision;
	
	if (isZero(self)) {
		std_Number_clearLeadingZeros(self); // Ensure it's truly 0.0
		return OK;
	}

	u32 digits_count = std.List.Size(priv.digits);
	if (digits_count > new_precision) std.List.Pop(priv.digits, digits_count);
	
	std_Number_clearLeadingZeros(self);
return OK;
}
errvt std_Number_AlignExponents(std_Number* a, std_Number* b) {
    
return	(apriv.exponent > bpriv.exponent) ? std_Number_shiftDigitsRight(b, b, apriv.exponent - bpriv.exponent) :
	(bpriv.exponent > apriv.exponent) ? std_Number_shiftDigitsRight(a, a, bpriv.exponent - apriv.exponent) :
	OK;
}
errvt moduleMethod(std_Number, FloatSubtract, std_Number* other, std_Number* result);
errvt moduleMethod(std_Number, FloatAdd, std_Number* other, std_Number* result) {
    
	// 0 case is handled by the top level std_Number.Add call
	if (priv.sign != opriv.sign) {
		std_Number* temp_other = makeTempNum(opriv.digits, opriv.precision);
		temp_other->__private.sign *= -1;
		return std_Number_FloatSubtract(self, temp_other, result);
	}

	std_Number
	* temp_other = makeTempNum(copy(opriv.digits, new_alloc(std_List)), opriv.precision),
	* temp_self  = makeTempNum(copy(opriv.digits, new_alloc(std_List)), priv.precision);
	
	iferr(std_Number_AlignExponents(temp_self, temp_other)){
		return err;
	}
	iferr(std_Number_absoluteAdd(result, temp_self, temp_other)){
		return err;
	}

	rpriv.floating = 1;
	rpriv.exponent = temp_self->__private.exponent;
	rpriv.sign     = temp_self->__private.sign;
	
	del(temp_self->__private.digits);
	del(temp_other->__private.digits);
	
return OK;
}

errvt moduleMethod(std_Number, FloatSubtract, std_Number* other, std_Number* result){
	
	// 0 case is handled by the top level std_Number.Subtract call
	if (priv.sign != opriv.sign) {
		std_Number* temp_other = makeTempNum(opriv.digits, opriv.precision);
		temp_other->__private.sign *= -1;
		return std_Number_FloatAdd(self, temp_other, result);
	}

	std_Number
	* temp_other = makeTempNum(copy(opriv.digits, new_alloc(std_List)), opriv.precision),
	* temp_self  = makeTempNum(copy(opriv.digits, new_alloc(std_List)), priv.precision);
	
	iferr(std_Number_AlignExponents(temp_self, temp_other)){
		return err;
	}
	switch(std_Number_absoluteCompare(temp_self, temp_other)){
	case NUM_GREATER:{
		iferr(std_Number_absoluteSub(self, temp_self, temp_other)){return err;}
		rpriv.sign 	= temp_self->__private.sign;
	break;}
	case NUM_EQUALS:
	case NUM_LESSER:{
		iferr(std_Number_absoluteSub(self, temp_other, temp_self)){return err;}
		rpriv.sign 	= temp_self->__private.sign * -1;
	break;}
	default:{return ERR(ERR.INVALID, "invalid comparision");}
	}

	rpriv.floating = 1;
	rpriv.exponent = temp_self->__private.exponent;
	
	del(temp_self->__private.digits);
	del(temp_other->__private.digits);
	
return OK;
}

errvt moduleMethod(std_Number, FloatMultiply, std_Number* other, std_Number* result) {
    // Perform integer multiplication on the digits
    errvt status = std_Number_absoluteMultiply(result, self, other);
    rpriv.floating = 1;
    rpriv.exponent = priv.exponent + opriv.exponent;
    rpriv.sign = (priv.sign == opriv.sign) ? 1 : -1;
    return status;

}

errvt moduleMethod(std_Number, FloatDivide, std_Number* other, std_Number* remainder, std_Number* result){

	if (isZero(other)) {
		return ERR(ERR.INVALID, "cannot divide by 0");
	}else if (isZero(self)) {
		return OK;
	}
	
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
		* tempRemainder = makeTempNum(pushList(u32, 10), precision), 
		* tempProduct	= makeTempNum(pushList(u32, 10), precision);
	
	rpriv.exponent = priv.exponent - opriv.exponent;

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

	std_Number* productQuotientB = tempRemainder; // reusing temp remainders list to avoid extra allocations
	
	// Remainder = a - (quotient_temp * b)
	std_Number_FloatMultiply(result, other, productQuotientB);	
	std_Number_FloatSubtract(self, productQuotientB, remainder);
	
	// The sign of the remainder should be the same as the sign of the dividend (a).
	rpriv.sign = priv.sign;
	std_Number_clearLeadingZeros(remainder);
	
	// Determine sign of the result
	rpriv.sign = (priv.sign == opriv.sign) ? 1 : -1;

exit:	
	pop(tempProduct->__private.digits);
	pop(tempRemainder->__private.digits);
	
return reserr;
}

#undef module

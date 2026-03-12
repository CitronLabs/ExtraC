#pragma once
#include <XC.Core/pkg.c>
#include "../__Internal/pkg.c"

#ifndef HIDE_USE
from(std,
    use(Number),
    use(Stream)
)
alias(std.Stream.Process, then);
#endif

u64 moduleMethod(std_Number, FloatScan, Number_FormatArgs* format, Stream* in) {
	
	std.Number.setZero(self); // Initialize to 0.0
	
	rune c = 0;
	len_t prev_pos = size(in);

	std.Stream.Process
	   .start(in)
	   .doDecode(std.String.Encoding.UTF8.Decoder, c){

		// 1. Handle sign
		if   (c == '-') { priv.sign = -1; then.next();}
		elif (c == '+') { priv.sign = 1;  then.next();}
		else { priv.sign = 1;	}
	   
		// Temporary stream to build the mantissa digits
		Stream* mantissa = push(Stream, .ops.frameSize = sizeof(u8));

		if(mantissa == nil){
			ERR(ERR.INIT, "could not initialize mantissa buff");
			return 0;
		}
		
		i64 decimal_point_pos = -1; // Position of decimal point relative to start of digits
		i64 initial_mantissa_len = 0; // Length of integer part of mantissa


		rune encode_point = 0;

		std.Stream.Process
		    .start(mantissa)
		    .run(({
			while(iswdigit(c)){
			    std.String.Encoding.UTF8.encode((char*)&encode_point, c);
			    then.writeData(
					&encode_point, 
					std.String.Encoding.UTF8.charSize(encode_point)
			    );
			    then.next();
			}
			if(c == '.'){

				decimal_point_pos = initial_mantissa_len; // Record decimal point position
				then.next();

				while (iswdigit(c)) {
				    std.String.Encoding.UTF8.encode((char*)&encode_point, c);
				    then.writeData(
						&encode_point, 
						std.String.Encoding.UTF8.charSize(encode_point)
				    );
				    then.next();
				}
			}

			run_continue;
		    })) 
		    .end();
			
		// If no digits were found at all (e.g., just ".", "+.", "-."), treat as zero
		if (len(mantissa) == 0) {
			pop(mantissa);
			std.Number.setZero(self);
			then.end();
			return prev_pos - size(in);
		}
		
		Internal.IntScan(self, nil, mantissa);
		
		// Calculate initial exponent based on decimal point
		if (decimal_point_pos != -1) {
			// Example: "123.45" -> mantissa "12345", decimal_point_pos 3.
			// Exponent should be -2 (12345 * 10^-2 = 123.45)
			priv.exponent = decimal_point_pos - 
				(i64)strnlen((strc8)std.Stream.GetPointer(mantissa), size(mantissa));
		} else {
			// No decimal point, exponent is 0
			priv.exponent = 0;
		}

		then.next();
		
		// 4. Parse exponent part (e.g., "e+5", "E-2")
		if (c == 'e' || c == 'E') {
			then.next();
			int exp_sign = 1;

			if (c == '-') {
				exp_sign = -1;
			}
			then.next();
		
			i64 parsed_exp_val = 0;
			char* endptr;
			// Use strtol for parsing the exponent value
			
			if (iswdigit(c)) 
			    scanFrom(in, $(parsed_exp_val));
			    

			priv.exponent += (i64)parsed_exp_val * exp_sign;
		}
		
		Internal.clearLeadingZeros(self); 

		pop(mantissa);

	   } then.end();
	
return prev_pos - size(in);
}

#undef module

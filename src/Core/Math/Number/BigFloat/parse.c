#define module std, Number
#include "../../../pkg.h"
#include "../utils.h"

import(std)


#ifndef HIDE_USE


from(std,
    use(Number),
    use(Stream)
)
#endif

u64 moduleMethod(std_Number, IntScan, Number_FormatArgs* format, Stream* in);
u64 moduleMethod(std_Number, FloatScan, Number_FormatArgs* format, Stream* in) {
	
	std_Number_setZero(self); // Initialize to 0.0
	
	rune c = 0;
	len_t prev_pos = size(in);

	std.Stream.Process
	   .start(in)
	   .doDecode(std.String.UTF8.Decoder, c){

		// 1. Handle sign
		if   (c == '-') { priv.sign = -1; process->next();}
		elif (c == '+') { priv.sign = 1;  process->next();}
		else { priv.sign = 1;	}
	   
		// Temporary stream to build the mantissa digits
		Stream* mantissa = push(Stream, .ops.frameSize = sizeof(u8));

		if(mantissa == nil){
			ERR(ERR_INITFAIL, "could not initialize mantissa buff");
			return 0;
		}
		
		i64 decimal_point_pos = -1; // Position of decimal point relative to start of digits
		i64 initial_mantissa_len = 0; // Length of integer part of mantissa


		rune encode_point = 0;

		std.Stream.Process
		    .start(mantissa)
		    .doRun(1){
			while(iswdigit(c)){
			    std.String.UTF8.encode((char*)&encode_point, c);
			    process->writeData(
					&encode_point, 
					std.String.UTF8.charSize(encode_point)
			    );
			    process->next();
			}
		    } 
		    then.doRun(c == '.'){
			decimal_point_pos = initial_mantissa_len; // Record decimal point position
			process->next();

			while (iswdigit(c)) {
			    std.String.UTF8.encode((char*)&encode_point, c);
			    process->writeData(
					&encode_point, 
					std.String.UTF8.charSize(encode_point)
			    );
			    process->next();
			}
		    }
		    then.end();
			
		// If no digits were found at all (e.g., just ".", "+.", "-."), treat as zero
		if (size(mantissa) == 0) {
			pop(mantissa);
			std_Number_setZero(self);
			process->end();
			return prev_pos - size(in);
		}
		
		std_Number_IntScan(self, nil, mantissa);
		
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

		process->next();
		
		// 4. Parse exponent part (e.g., "e+5", "E-2")
		if (c == 'e' || c == 'E') {
			process->next();
			int exp_sign = 1;

			if (c == '-') {
				exp_sign = -1;
			}
			process->next();
		
			i64 parsed_exp_val = 0;
			char* endptr;
			// Use strtol for parsing the exponent value
			
			if (iswdigit(c)) 
			    scanFrom(in, $(parsed_exp_val));
			    

			priv.exponent += (i64)parsed_exp_val * exp_sign;
		}
		
		std_Number_clearLeadingZeros(self); 

		pop(mantissa);

	   } then.end();
	
return prev_pos - size(in);
}

#undef module

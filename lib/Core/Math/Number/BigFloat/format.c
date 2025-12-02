#include "../../../pkg.h"
#include "../utils.h"

import(std)


u64 methodimpl(std_Number, IntPrintDeci, std_Stream* out);
u64 methodimpl(std_Number, FloatPrint, std_Stream* out) {

    	u64 formatted_len = 0;
	u64 mantissa_len = 0;

    	// 1. Append sign
    	if (priv.sign == -1) {
        	write(out, "-", fmt_end);
        	formatted_len++;
    	}

    	// 2. Get mantissa string
	var mantissa_strm = push(std_Stream);
	if(mantissa_strm == nil){
		ERR(ERR_INITFAIL, "could not initialize stream for mantissa");
		return 0;
	}

	if((mantissa_len = std_Number_IntPrintDeci(self, mantissa_strm)) == 0){
		pop(mantissa_strm);
		ERR(ERR_FAIL, "failed to parse mantissa");
		return 0;
	}

	u64 integer_part_len = mantissa_len + priv.exponent; // Number of digits before decimal point

	// Case: 0.00...mantissa (e.g., 0.00123)
	if (integer_part_len <= 0) {
		formatted_len += write(out, "0.", fmt_end);
		
		// Add leading zeros after decimal point
		for (i64 i = 0; i < -integer_part_len; i++) {
			write(out, "0", fmt_end);
		}
		formatted_len += -integer_part_len;
		formatted_len += write(out, $(mantissa_strm), fmt_end);

	// Case: mantissa.00... (e.g., 123.0 or 123000.0)
	} else if (integer_part_len >= mantissa_len) {
		formatted_len += write(out, $(mantissa_strm), fmt_end);
		
		// Add trailing zeros if exponent is positive
		for (i64 i = 0; i < priv.exponent; i++) {
			write(out, "0", fmt_end);
			formatted_len++;
		}
		formatted_len += write(out, ".0", fmt_end); // Always append .0 for integer representations
		
	// Case: integer.decimal (e.g., 123.45)
	} else {
		std_String* mantissa_string = push(std_String);

		if(mantissa_string == nil){
			ERR(ERR_INITFAIL, "failed to initialize string for mantissa stream");
			pop(mantissa_strm);
			return 0;
		}

		if(!scanFrom(mantissa_strm, $(mantissa_string))){
			ERR(ERR_FAIL, "failed to decode string from mantissa stream");
			pop(mantissa_string, mantissa_strm);
			return 0;
		}

		formatted_len += write(out, 
			  std.String.View(mantissa_string, 0, integer_part_len), 
			  ".",
			  std.String.View(mantissa_string, integer_part_len, mantissa_len),
		fmt_end);
	}
	
	pop(mantissa_strm);

return formatted_len;
}

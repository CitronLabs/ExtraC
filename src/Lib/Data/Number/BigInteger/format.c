#pragma once
#include <XC.Core/pkg.c>
#include "../__Internal/pkg.c"

static u32 std_Number_divide_smallInt(std_Number* self, u32 divisor) {
	if (priv.sign == 0 || divisor == 0) {
		return 0;
	}
	
	u64 digits_count = std.List.Size(priv.digits);
	u64 remainder = 0;
		
	u32* digits = std.List.GetPointer(priv.digits, 0);

	for (int i = digits_count - 1; i >= 0; i--) {
		u32* current_digit_val = index(priv.digits, i);
		
		u64 combined_val = (remainder << 32) + *current_digit_val;
		
		u32 quotient_digit = (u32)(combined_val / divisor);
		remainder = combined_val % divisor;
	
		digits[i] = quotient_digit;
	}
	
	Internal.clearLeadingZeros(self);
	return (u32)remainder;
}

u64 moduleMethod(std_Number, IntPrintDeci, std_Stream* out) {
	
	const u32 POWER_OF_10 = 1000000000; // 10^9
	const int CHUNK_SIZE = 9;
	u64 formatted_len = 0;

	std_List* temp_list = pushList(u32, 10);
	std_Number* temp_num = makeTempNum(temp_list, priv.precision);
	Internal.Copy(temp_num, self);
	
	List(u32) digit_chunks = pushList(u32, 10);
	
	while(temp_num->__private.sign != 0) {
		u32 remainder = Internal.divide_smallInt(temp_num, POWER_OF_10);
		std.List.Append(digit_chunks, &remainder, 1);
	}
	
	// The first chunk is at the end of the list and doesn't need padding
	u32 chunk_val = 0;
	char buffer[CHUNK_SIZE + 1];
	
	chunk_val = *(u32*)index(digit_chunks, len(digit_chunks) - 1);
	
	formatted_len += printTo(out, $F(chunk_val, .precision = CHUNK_SIZE));
	
	// Append remaining chunks with padding
	for (int i = len(digit_chunks) - 2; i >= 0; i--) {
		chunk_val = *(u32*)index(digit_chunks, len(digit_chunks) - 1);
		formatted_len += printTo(out, $F(chunk_val, .precision = CHUNK_SIZE));
	}
	pop(digit_chunks);
	pop(temp_list);

return formatted_len;
}

u64 moduleMethod(std_Number, IntPrintHex, std_Stream* out) {
	
	u64 formatted_len = 0;
	u64 num_digits = std.List.Size(priv.digits);
	char buffer[(sizeof(u32) * 2) + 1] = {0};
	
	printTo(out, "0x");
	
	u32 digit = 0;
	
	digit = *(u32*)index(priv.digits, len(priv.digits) - 1);
	
	formatted_len += printTo(out, $F(digit, .base = 6));
	
	for (int i = num_digits - 2; i >= 0; i--) {
		std.List.Index(priv.digits, LISTINDEX_READ, i, 1, &digit);

		formatted_len += printTo(out, $F(digit, .base = 6));
	}
return formatted_len;
}

u64 moduleMethod(std_Number, IntPrintBin, std_Stream* out) {
	
	u64 formatted_len = 0;
	u64 num_digits = std.List.Size(priv.digits);
	char buffer[(sizeof(u32) * 8) + 1] = {0};
	
	printTo(out, "0b");
	
	// Print the most significant digit first, without padding
	u32 digit = 0;
	std.List.Index(priv.digits, LISTINDEX_READ, num_digits - 1, 1, &digit);
	
	// Find the first '1' to avoid leading zeros
	u32 idx = 0;
	u32 start_bit = 31;
	while(start_bit > 0 && !((digit >> start_bit) & 1)) {
		start_bit--;
	}
	for(int i = start_bit; i >= 0; i--) {
		buffer[idx++] = ((digit >> i) & 1) ? '1' : '0';
	}
	if (idx == 0) buffer[idx++] = '0'; // Handle the case of the number 0
		buffer[idx] = '\0';
	
	formatted_len += write(out, asString(buffer, sizeof(u32) * 8));
		
	for (int i = num_digits - 2; i >= 0; i--) {
		std.List.Index(priv.digits, LISTINDEX_READ, i, 1, &digit);
		
		u32 idx = 0;
		for(int j = 31; j >= 0; j--) {
			buffer[idx++] = ((digit >> j) & 1) ? '1' : '0';
		}
		buffer[idx] = '\0';
		formatted_len += printTo(out, buffer);
	}

return formatted_len;
}

#undef module

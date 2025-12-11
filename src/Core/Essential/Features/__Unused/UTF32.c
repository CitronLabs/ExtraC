#pragma once
#define __STRING_ENCODING_UTILS__
#include "utils.h"
#include "../../../../pkg.h"

import(std)


	
// UTF-32 to UTF-8 conversion
len_t UTF32_toUtf8(c32* in, len_t in_max, c8* dest, len_t dest_max){

	// Max size is 4 bytes per codepoint

	u64 i = 0;
	
    	while (*in && i < dest_max && i < in_max) {
		u32 codepoint = in[i];
		u64 encoded_len = std.UTF8.encode(&dest[i], codepoint);
		if (encoded_len == 0) {
			ERR(STRINGERR_ENCODING, "Failed to encode UTF8 codepoint");
			return 0;
		}
		i += encoded_len;
	}


return i;
}

// UTF-32 to UTF-16 conversion
len_t UTF32_toUtf16(c32* in, len_t in_max, c16*   dest, len_t dest_max){

	u64 i = 0;
	
    	while (*in && i < dest_max && i < in_max) {
		u32 codepoint = in[i];
		u64 encoded_len = std.UTF16.encode(&dest[i], codepoint);
		if (encoded_len == 0) {
			ERR(STRINGERR_ENCODING, "Failed to encode UTF16 codepoint");
			return 0;
		}
		i += encoded_len;
	}

return i;
}

// UTF-32 to ASCII conversion
len_t UTF32_toAscii(c32* in, len_t in_max, char*  dest, len_t dest_max){

	u64 i = 0;
	
    	while (*in && i < dest_max && i < in_max) {
		u32 codepoint = in[i];
		if (codepoint <= ASCII_MAX) {
			dest[i++] = (char)codepoint;
		} else {
			dest[i++] = ASCII_REPLACEMENT_CHAR;
		}
	}
	
return i;
}



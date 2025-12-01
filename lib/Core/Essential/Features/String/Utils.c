#include "../../../../pkg.h"
#include "Encodings/utils.h"

import(std)


static inline len_t UTF8_strnlen(void* in, len_t len) {
	const char* str = in;
	len_t length = 0;
	while (*str != '\0' && len >= length) {
		if ((*str & UTF8_1BYTE_MASK) == UTF8_1BYTE_HEADER) {
			str++;
		} else if ((*str & UTF8_2BYTE_MASK) == UTF8_2BYTE_HEADER) {
			str += 2;
		} else if ((*str & UTF8_3BYTE_MASK) == UTF8_3BYTE_HEADER) {
			str += 3;
		} else if ((*str & UTF8_4BYTE_MASK) == UTF8_4BYTE_HEADER) {
			str += 4;
		} else {
			ERR(ERR_INVALID, "invalid utf8 string");
			return 0;
		}
		length++;
	}
return length;
}

static inline len_t UTF8_strncmp(void* str1, void* str2, len_t len){

	char
	    * s1 = str1,
	    * s2 = str2
	;

	len_t len_compared = 0;

	while (*s1 != '\0' && *s2 != '\0' && len_compared < len) {
		// Decode codepoint for s1
		u32 codepoint1 = 0, codepoint2 = 0;
		
		if(std.UTF8.decode(&s1, &codepoint1) != OK &&
		   std.UTF8.decode(&s2, &codepoint2) != OK
		){
			return false;
		}

		
		if (codepoint1 != codepoint2) return false;
		
		len_compared++;
	}
	
	if (len_compared == len || (*s1 == '\0' && *s2 == '\0')) {
	    return true;
	}
	
	if (*s1 == '\0') {
	    return false;
	}

return false;
}

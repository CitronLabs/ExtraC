#pragma once
#include "../strings.h"

static inline size_t utf8_strnlen(const char *str, size_t len) {
	size_t length = 0;
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

static inline errvt decodeUTF8CodePoint(char** start, u32* codepoint){

	char* encoding = *start;

	*codepoint = 
		  (*encoding & UTF8_1BYTE_MASK) == UTF8_1BYTE_HEADER  ?
			  *encoding++ 
      		: (*encoding & UTF8_2BYTE_MASK) == UTF8_2BYTE_HEADER  ?
      			((*encoding++ & UTF8_2BYTE_PREFIX) 	<< 6)  | 
      			 (*encoding++ & UTF8_CONT_BYTE_MASK)
      		: (*encoding & UTF8_3BYTE_MASK) == UTF8_3BYTE_HEADER  ?
      			((*encoding++ & UTF8_3BYTE_PREFIX) 	<< 12) | 
      			((*encoding++ & UTF8_CONT_BYTE_MASK)    << 6)  | 
      			 (*encoding++ & UTF8_CONT_BYTE_MASK)
      		: (*encoding & UTF8_4BYTE_MASK) == UTF8_4BYTE_HEADER  ?
      			((*encoding++ & UTF8_4BYTE_PREFIX) 	<< 18) | 
      			((*encoding++ & UTF8_CONT_BYTE_MASK)   	<< 12) | 
      			((*encoding++ & UTF8_CONT_BYTE_MASK)    << 6)  | 
      			 (*encoding++ & UTF8_CONT_BYTE_MASK)
      		: maxof(u32)
	;

	if(*codepoint == maxof(u32)){
	    encoding++;
	    return ERR(ERR_INVALID, "string is an invalid utf8 string");
	}

	*start = encoding;

return OK;
}


// Helper function to encode a UTF-8 codepoint
static inline u64 encode_utf8(char *dest, u32 codepoint) {
    if (codepoint <= UTF8_1BYTE_MAX) {
        dest[0] = (char)codepoint;
        return UTF8_1BYTE_SIZE;
    } else if (codepoint <= UTF8_2BYTE_MAX) {
        dest[0] = (char)(UTF8_2BYTE_PREFIX | (codepoint >> 6));
        dest[1] = (char)(UTF8_CONT_BYTE_MASK | (codepoint & UTF8_CONT_BYTE_BITS));
        return UTF8_2BYTE_SIZE;
    } else if (codepoint <= UTF8_3BYTE_MAX) {
        dest[0] = (char)(UTF8_3BYTE_PREFIX | (codepoint >> 12));
        dest[1] = (char)(UTF8_CONT_BYTE_MASK | ((codepoint >> 6) & UTF8_CONT_BYTE_BITS));
        dest[2] = (char)(UTF8_CONT_BYTE_MASK | (codepoint & UTF8_CONT_BYTE_BITS));
        return UTF8_3BYTE_SIZE;
    } else if (codepoint <= UTF8_4BYTE_MAX) {
        dest[0] = (char)(UTF8_4BYTE_PREFIX | (codepoint >> 18));
        dest[1] = (char)(UTF8_CONT_BYTE_MASK | ((codepoint >> 12) & UTF8_CONT_BYTE_BITS));
        dest[2] = (char)(UTF8_CONT_BYTE_MASK | ((codepoint >> 6) & UTF8_CONT_BYTE_BITS));
        dest[3] = (char)(UTF8_CONT_BYTE_MASK | (codepoint & UTF8_CONT_BYTE_BITS));
        return UTF8_4BYTE_SIZE;
    }
    return 0; // Invalid codepoint
}

// UTF-8 to UTF-16 conversion
inst(String) convert_utf8_to_utf16(inst(String) self) {
	if (self->txt.type != CHAR_UTF8) {
		ERR(STRINGERR_ENCODING, "Source string is not UTF8");
		return NULL;
	}

	// Estimate max size, UTF-8 to UTF-16 can double in size (1 to 2)
	u64 max_len = self->__private->len_bytes * UTF8_TO_UTF16_MAX_FACTOR;
	char16_t* new_data = calloc(max_len, sizeof(char16_t));
	if (!new_data) {
		ERR(DATAERR_MEMALLOC, "Failed to allocate memory for UTF16 string");
		return NULL;
	}

	char* utf8_ptr = self->txt.data.utf8;
	u64 new_len = 0;
	u32 codepoint;

	while (*utf8_ptr) {
		if (decodeUTF8CodePoint(&utf8_ptr, &codepoint) != OK) {
			free(new_data);
			ERR(STRINGERR_ENCODING, "Invalid UTF8 sequence");
			return NULL;
		}

		u64 encoded_len = encode_utf16(&new_data[new_len], codepoint);
		if (encoded_len == 0) {
			free(new_data);
			ERR(STRINGERR_ENCODING, "Failed to encode UTF16 codepoint");
			return NULL;
		}
		new_len += encoded_len;
	}

	inst(String) new_string = new(String, .inline_alloc = true, .txt = { .data.utf16 = new_data, .type = CHAR_UTF16 });
	new_string->__private->len_bytes = new_len * sizeof(char16_t);
	new_string->len = new_len;

	return new_string;
}


// UTF-8 to UTF-32 conversion
inst(String) convert_utf8_to_utf32(inst(String) self) {
	if (self->txt.type != CHAR_UTF8) {
		ERR(STRINGERR_ENCODING, "Source string is not UTF8");
		return NULL;
	}

	// Max size is `self->len_bytes` since a 1-byte UTF-8 char becomes a 4-byte UTF-32 char
	u64 max_len = self->__private->len_bytes;
	char32_t* new_data = calloc(max_len + sizeof('\0'), sizeof(char32_t));
	if (!new_data) {
		ERR(DATAERR_MEMALLOC, "Failed to allocate memory for UTF32 string");
		return NULL;
	}

	char* utf8_ptr = self->txt.data.utf8;
	u64 new_len = 0;
	u32 codepoint;

	while (*utf8_ptr) {
		if (decodeUTF8CodePoint(&utf8_ptr, &codepoint) != OK) {
			free(new_data);
			ERR(STRINGERR_ENCODING, "Invalid UTF8 sequence");
			return NULL;
		}
		new_data[new_len++] = codepoint;
	}
	
	inst(String) new_string = new(String, .inline_alloc = true, .txt = { .data.utf32 = new_data, .type = CHAR_UTF32 });
	new_string->__private->len_bytes = new_len * sizeof(char32_t);
	new_string->len = new_len;

	return new_string;
}

// UTF-8 to ASCII conversion
inst(String) convert_utf8_to_ascii(inst(String) self) {
	if (self->txt.type != CHAR_UTF8) {
		ERR(STRINGERR_ENCODING, "Source string is not UTF8");
		return NULL;
	}

	// ASCII is 1-byte per character, so max size is length of self
	u64 max_len = self->len;
	char* new_data = calloc(max_len + 1, sizeof(char));
	if (!new_data) {
		ERR(DATAERR_MEMALLOC, "Failed to allocate memory for ASCII string");
		return NULL;
	}

	char* utf8_ptr = self->txt.data.utf8;
	u64 new_len = 0;
	u32 codepoint;

	while (*utf8_ptr) {
		if (decodeUTF8CodePoint(&utf8_ptr, &codepoint) != OK) {
			free(new_data);
			ERR(STRINGERR_ENCODING, "Invalid UTF8 sequence");
			return NULL;
		}

		if (codepoint <= ASCII_MAX) {
			new_data[new_len++] = (char)codepoint;
		} else {
			// Replace non-ASCII characters with '?'
			new_data[new_len++] = ASCII_REPLACEMENT_CHAR;
		}
	}

	inst(String) new_string = new(String, .inline_alloc = true, .txt = { .data.utf8 = new_data, .type = CHAR_ASCII });
	new_string->__private->len_bytes = new_len;
	new_string->len = new_len;

	return new_string;
}

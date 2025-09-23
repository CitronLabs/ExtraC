#pragma once
#include "../strings.h"

static inline size_t str32nlen(char32_t* str, size_t maxlen){
	size_t len = 0;
	while (str[len] != u'\0' || len == maxlen) {
		len++;
	}
return len;
}

// UTF-32 to UTF-8 conversion
inst(String) convert_utf32_to_utf8(inst(String) self) {
	if (self->txt.type != CHAR_UTF32) {
		ERR(STRINGERR_ENCODING, "Source string is not UTF32");
		return NULL;
	}

	// Max size is 4 bytes per codepoint
	u64 max_len = self->len * 4 + 1;
	char* new_data = calloc(max_len, sizeof(char));
	if (!new_data) {
		ERR(DATAERR_MEMALLOC, "Failed to allocate memory for UTF8 string");
		return NULL;
	}

	char32_t* utf32_ptr = self->txt.data.utf32;
	u64 new_len = 0;
	
	for (u64 i = 0; i < self->len; i++) {
		u32 codepoint = utf32_ptr[i];
		u64 encoded_len = encode_utf8(&new_data[new_len], codepoint);
		if (encoded_len == 0) {
			free(new_data);
			ERR(STRINGERR_ENCODING, "Failed to encode UTF8 codepoint");
			return NULL;
		}
		new_len += encoded_len;
	}

	inst(String) new_string = new(String, .inline_alloc = true, .txt = { .data.utf8 = new_data, .type = CHAR_UTF8 });
	new_string->__private->len_bytes = new_len;
	new_string->len = utf8_strnlen(new_data, new_len);

	return new_string;
}

// UTF-32 to UTF-16 conversion
inst(String) convert_utf32_to_utf16(inst(String) self) {
	if (self->txt.type != CHAR_UTF32) {
		ERR(STRINGERR_ENCODING, "Source string is not UTF32");
		return NULL;
	}

	// Max size is 2 words per codepoint for surrogates
	u64 max_len = self->len * 2 + 1;
	char16_t* new_data = calloc(max_len, sizeof(char16_t));
	if (!new_data) {
		ERR(DATAERR_MEMALLOC, "Failed to allocate memory for UTF16 string");
		return NULL;
	}

	char32_t* utf32_ptr = self->txt.data.utf32;
	u64 new_len = 0;
	
	for (u64 i = 0; i < self->len; i++) {
		u32 codepoint = utf32_ptr[i];
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

// UTF-32 to ASCII conversion
inst(String) convert_utf32_to_ascii(inst(String) self) {
	if (self->txt.type != CHAR_UTF32) {
		ERR(STRINGERR_ENCODING, "Source string is not UTF32");
		return NULL;
	}

	u64 max_len = self->len;
	char* new_data = calloc(max_len, sizeof(char));
	if (!new_data) {
		ERR(DATAERR_MEMALLOC, "Failed to allocate memory for ASCII string");
		return NULL;
	}

	char32_t* utf32_ptr = self->txt.data.utf32;
	u64 new_len = 0;
	
	for (u64 i = 0; i < self->len; i++) {
		u32 codepoint = utf32_ptr[i];
		if (codepoint <= ASCII_MAX) {
			new_data[new_len++] = (char)codepoint;
		} else {
			new_data[new_len++] = ASCII_REPLACEMENT_CHAR;
		}
	}
	
	inst(String) new_string = new(String, .inline_alloc = true, .txt = { .data.utf8 = new_data, .type = CHAR_ASCII });
	new_string->__private->len_bytes = new_len;
	new_string->len = new_len;

	return new_string;
}


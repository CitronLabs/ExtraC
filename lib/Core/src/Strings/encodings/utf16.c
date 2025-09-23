#pragma once
#include "../strings.h"

static inline size_t str16nlen(char16_t* str, size_t maxlen){
	size_t len = 0;
	while (str[len] != u'\0' || len == maxlen) {
		len++;
	}
return len;
}

// Helper function to encode a UTF-16 codepoint
static inline u64 encode_utf16(char16_t *dest, u32 codepoint) {
    if (codepoint <= UTF8_3BYTE_MAX) { // Corresponds to the basic multilingual plane
        *dest = (char16_t)codepoint;
        return 1;
    } else if (codepoint <= UTF8_4BYTE_MAX) { // Supplementary plane
        codepoint -= UTF16_SURROGATE_OFFSET_BASE;
        dest[0] = (char16_t)(UTF16_HIGH_SURROGATE_START | (codepoint >> UTF16_HIGH_SURROGATE_SHIFT));
        dest[1] = (char16_t)(UTF16_LOW_SURROGATE_START | (codepoint & UTF16_LOW_SURROGATE_BITS));
        return 2;
    }
    return 0; // Invalid codepoint
}

inst(String) convert_utf16_to_utf8(inst(String) self) {
    if (self->txt.type != CHAR_UTF16) {
        ERR(STRINGERR_ENCODING, "Source string is not UTF16");
        return NULL;
    }

    // Estimate max size, UTF-16 to UTF-8 can be up to 3 bytes per 16-bit code unit
    u64 max_len = self->len * UTF16_TO_UTF8_MAX_FACTOR + sizeof('\0');
    char* new_data = calloc(max_len, sizeof(char));
    if (!new_data) {
        ERR(DATAERR_MEMALLOC, "Failed to allocate memory for UTF8 string");
        return NULL;
    }

    char16_t* utf16_ptr = self->txt.data.utf16;
    u64 new_len = 0;

    for (u64 i = 0; i < self->len; i++) {
        u32 codepoint;
        char16_t current_char = utf16_ptr[i];

        if (current_char >= UTF16_HIGH_SURROGATE_START && current_char <= UTF16_HIGH_SURROGATE_END) { // High surrogate
            if (i + 1 >= self->len) {
                free(new_data);
                ERR(STRINGERR_ENCODING, "Incomplete UTF16 surrogate pair");
                return NULL;
            }
            char16_t next_char = utf16_ptr[i+1];
            if (next_char >= UTF16_LOW_SURROGATE_START && next_char <= UTF16_LOW_SURROGATE_END) { // Low surrogate
                codepoint = (current_char - UTF16_HIGH_SURROGATE_OFFSET) * 0x400 + (next_char - UTF16_LOW_SURROGATE_OFFSET) + UTF16_SURROGATE_OFFSET_BASE;
                i++;
            } else {
                free(new_data);
                ERR(STRINGERR_ENCODING, "Invalid UTF16 surrogate pair");
                return NULL;
            }
        } else {
            codepoint = current_char;
        }

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
// UTF-16 to UTF-32 conversion
inst(String) convert_utf16_to_utf32(inst(String) self) {
    if (self->txt.type != CHAR_UTF16) {
        ERR(STRINGERR_ENCODING, "Source string is not UTF16");
        return NULL;
    }

    u64 max_len = self->len;
    char32_t* new_data = calloc(max_len, sizeof(char32_t));
    if (!new_data) {
        ERR(DATAERR_MEMALLOC, "Failed to allocate memory for UTF32 string");
        return NULL;
    }

    char16_t* utf16_ptr = self->txt.data.utf16;
    u64 new_len = 0;

    for (u64 i = 0; i < self->len; i++) {
        u32 codepoint;
        char16_t current_char = utf16_ptr[i];

        if (current_char >= UTF16_HIGH_SURROGATE_START && current_char <= UTF16_HIGH_SURROGATE_END) { // High surrogate
            if (i + 1 >= self->len) {
                free(new_data);
                ERR(STRINGERR_ENCODING, "Incomplete UTF16 surrogate pair");
                return NULL;
            }
            char16_t next_char = utf16_ptr[i+1];
            if (next_char >= UTF16_LOW_SURROGATE_START && next_char <= UTF16_LOW_SURROGATE_END) { // Low surrogate
                codepoint = (current_char - UTF16_HIGH_SURROGATE_OFFSET) * 0x400 + (next_char - UTF16_LOW_SURROGATE_OFFSET) + UTF16_SURROGATE_OFFSET_BASE;
                i++;
            } else {
                free(new_data);
                ERR(STRINGERR_ENCODING, "Invalid UTF16 surrogate pair");
                return NULL;
            }
        } else {
            codepoint = current_char;
        }
        new_data[new_len++] = codepoint;
    }
    
    inst(String) new_string = new(String, .inline_alloc = true, .txt = { .data.utf32 = new_data, .type = CHAR_UTF32 });
    new_string->__private->len_bytes = new_len * sizeof(char32_t);
    new_string->len = new_len;

    return new_string;
}
// UTF-16 to ASCII conversion
inst(String) convert_utf16_to_ascii(inst(String) self) {
 if (self->txt.type != CHAR_UTF16) {
        ERR(STRINGERR_ENCODING, "Source string is not UTF16");
        return NULL;
    }

    u64 max_len = self->len;
    char* new_data = calloc(max_len, sizeof(char));
    if (!new_data) {
        ERR(DATAERR_MEMALLOC, "Failed to allocate memory for ASCII string");
        return NULL;
    }

    char16_t* utf16_ptr = self->txt.data.utf16;
    u64 new_len = 0;

    for (u64 i = 0; i < self->len; i++) {
        u32 codepoint;
        char16_t current_char = utf16_ptr[i];

        if (current_char >= UTF16_HIGH_SURROGATE_START && current_char <= UTF16_HIGH_SURROGATE_END) { // High surrogate
            if (i + 1 >= self->len) {
                free(new_data);
                ERR(STRINGERR_ENCODING, "Incomplete UTF16 surrogate pair");
                return NULL;
            }
            char16_t next_char = utf16_ptr[i+1];
            if (next_char >= UTF16_LOW_SURROGATE_START && next_char <= UTF16_LOW_SURROGATE_END) { // Low surrogate
                codepoint = (current_char - UTF16_HIGH_SURROGATE_OFFSET) * 0x400 + (next_char - UTF16_LOW_SURROGATE_OFFSET) + UTF16_SURROGATE_OFFSET_BASE;
                i++;
            } else {
                free(new_data);
                ERR(STRINGERR_ENCODING, "Invalid UTF16 surrogate pair");
                return NULL;
            }
        } else {
            codepoint = current_char;
        }
        
        if (codepoint <= ASCII_MAX) {
            new_data[new_len++] = (char)codepoint;
        } else {
            new_data[new_len++] = ASCII_REPLACEMENT_CHAR;
        }
    }
    
    inst(String) new_string = new(String, .inline_alloc = true, .txt = { .data.utf8 = new_data, .type = CHAR_ASCII });
    new_string->__private->len_bytes = new_len;
    new_string->len = new_len;

    return new_string;}

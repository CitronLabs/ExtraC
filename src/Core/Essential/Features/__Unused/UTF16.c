#pragma once
#define __STRING_ENCODING_UTILS__
#include "utils.h"

#include "../../../../pkg.h"

import(std)



static inline len_t str16nlen(c16* str, len_t maxlen){
	len_t len = 0;
	while (str[len] != u'\0' || len == maxlen) {
		len++;
	}
return len;
}

// Helper function to encode a UTF-16 codepoint
static inline u64 encode_utf16(c16 *dest, u32 codepoint) {
    if (codepoint <= UTF8_3BYTE_MAX) { // Corresponds to the basic multilingual plane
        *dest = (c16)codepoint;
        return 1;
    } else if (codepoint <= UTF8_4BYTE_MAX) { // Supplementary plane
        codepoint -= UTF16_SURROGATE_OFFSET_BASE;
        dest[0] = (c16)(UTF16_HIGH_SURROGATE_START | (codepoint >> UTF16_HIGH_SURROGATE_SHIFT));
        dest[1] = (c16)(UTF16_LOW_SURROGATE_START | (codepoint & UTF16_LOW_SURROGATE_BITS));
        return 2;
    }
return 0; // Invalid codepoint
}


len_t UTF16_toUtf8 (c16* in, len_t in_max, c8*  dest, len_t dest_max){

    u64 i = 0;

    while (*in && i < dest_max && i < in_max) {
        u32 codepoint;
        c16 current_char = in[i];

        if (current_char >= UTF16_HIGH_SURROGATE_START && current_char <= UTF16_HIGH_SURROGATE_END) { // High surrogate
            if (i + 1 >= in_max) {
                ERR(STRINGERR_ENCODING, "Incomplete UTF16 surrogate pair");
                return 0;
            }
            c16 next_char = in[i + 1];
            if (next_char >= UTF16_LOW_SURROGATE_START && next_char <= UTF16_LOW_SURROGATE_END) { // Low surrogate
                codepoint = (current_char - UTF16_HIGH_SURROGATE_OFFSET) * 0x400 + (next_char - UTF16_LOW_SURROGATE_OFFSET) + UTF16_SURROGATE_OFFSET_BASE;
		i++;
            } else {
                ERR(STRINGERR_ENCODING, "Invalid UTF16 surrogate pair");
                return 0;
            }
        } else {
            codepoint = current_char;
        }

        u64 encoded_len = std.UTF8.encode(&dest[i], codepoint);
        if (encoded_len == 0) {
            ERR(STRINGERR_ENCODING, "Failed to encode UTF8 codepoint");
            return 0;
        }
        i += encoded_len;
    }

return i;
}

// UTF-16 to UTF-32 conversion
len_t UTF16_toUtf32(c16* in, len_t in_max, c32* dest, len_t dest_max){

    u64 i = 0;

    while (*in && i < dest_max && i < in_max) {
        u32 codepoint;
        c16 current_char = in[i];

        if (current_char >= UTF16_HIGH_SURROGATE_START && current_char <= UTF16_HIGH_SURROGATE_END) { // High surrogate
            if (i + 1 >= in_max) {
                ERR(STRINGERR_ENCODING, "Incomplete UTF16 surrogate pair");
                return 0;
            }
            c16 next_char = in[i+1];
            if (next_char >= UTF16_LOW_SURROGATE_START && next_char <= UTF16_LOW_SURROGATE_END) { // Low surrogate
                codepoint = (current_char - UTF16_HIGH_SURROGATE_OFFSET) * 0x400 + (next_char - UTF16_LOW_SURROGATE_OFFSET) + UTF16_SURROGATE_OFFSET_BASE;
                i++;
            } else {
                ERR(STRINGERR_ENCODING, "Invalid UTF16 surrogate pair");
                return 0;
            }
        } else {
            codepoint = current_char;
        }
        dest[i++] = codepoint;
    }
    
return i;
}

// UTF-16 to ASCII conversion
len_t UTF16_toAscii(c16* in, len_t in_max, char* dest, len_t dest_max){

    u64 i = 0;

    while (*in && i < dest_max && i < in_max) {
        u32 codepoint;
        c16 current_char = in[i];

        if (current_char >= UTF16_HIGH_SURROGATE_START && current_char <= UTF16_HIGH_SURROGATE_END) { // High surrogate
            if (i + 1 >= in_max) {
                ERR(STRINGERR_ENCODING, "Incomplete UTF16 surrogate pair");
                return 0;
            }
            c16 next_char = in[i+1];
            if (next_char >= UTF16_LOW_SURROGATE_START && next_char <= UTF16_LOW_SURROGATE_END) { // Low surrogate
                codepoint = (current_char - UTF16_HIGH_SURROGATE_OFFSET) * 0x400 + (next_char - UTF16_LOW_SURROGATE_OFFSET) + UTF16_SURROGATE_OFFSET_BASE;
                i++;
            } else {
                ERR(STRINGERR_ENCODING, "Invalid UTF16 surrogate pair");
                return 0;
            }
        } else {
            codepoint = current_char;
        }
        
        if (codepoint <= ASCII_MAX) {
            dest[i++] = (char)codepoint;
        } else {
            dest[i++] = ASCII_REPLACEMENT_CHAR;
        }
    }
    
return i;
}

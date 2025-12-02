#pragma once
#include "utils.h"
#include "../../../../pkg.h"

import(std)


#undef rewind

// Helper function to encode a UTF-16 codepoint
static inline u64 encode_utf16(c16 *dest, rune codepoint) {
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

static inline errvt UTF8_decode(char** start, rune* codepoint){

	char* encoding = *start;
	rune c = 0;

	if(!codepoint)
		codepoint = &c;

	if((*encoding & UTF8_1BYTE_MASK) == UTF8_1BYTE_HEADER){
		*codepoint = *encoding++;
	}
	elif ((*encoding & UTF8_2BYTE_MASK) == UTF8_2BYTE_HEADER){
		*codepoint  =  (*encoding++ & UTF8_2BYTE_PREFIX)  << 6;
		*codepoint |=  (*encoding++ & UTF8_CONT_BYTE_MASK);
	}
      	elif((*encoding & UTF8_3BYTE_MASK) == UTF8_3BYTE_HEADER){
      		*codepoint  = (*encoding++ & UTF8_3BYTE_PREFIX)   << 12;
		*codepoint |= (*encoding++ & UTF8_CONT_BYTE_MASK) << 6;  
		*codepoint |= (*encoding++ & UTF8_CONT_BYTE_MASK);
	} 
      	elif((*encoding & UTF8_4BYTE_MASK) == UTF8_4BYTE_HEADER){
      		*codepoint = (*encoding++ & UTF8_4BYTE_PREFIX)    << 18; 
      		*codepoint = (*encoding++ & UTF8_CONT_BYTE_MASK)  << 12;
      		*codepoint = (*encoding++ & UTF8_CONT_BYTE_MASK)  << 6;
      		*codepoint = (*encoding++ & UTF8_CONT_BYTE_MASK);
	}
	else{	*codepoint = maxof(u32);    }

	if(*codepoint == maxof(u32)){
	    encoding++;
	    return ERR(ERR_INVALID, "string is an invalid utf8 string");
	}

	*start = encoding;

return OK;
}


// Helper function to encode a UTF-8 codepoint
static inline errvt UTF8_encode(char *dest, rune codepoint) {
    if (codepoint <= UTF8_1BYTE_MAX) {
        dest[0] = (char)codepoint;
        return OK;
    } else if (codepoint <= UTF8_2BYTE_MAX) {
        dest[0] = (char)(UTF8_2BYTE_PREFIX   | (codepoint >> 6));
        dest[1] = (char)(UTF8_CONT_BYTE_MASK | (codepoint & UTF8_CONT_BYTE_BITS));
        return OK;
    } else if (codepoint <= UTF8_3BYTE_MAX) {
        dest[0] = (char)(UTF8_3BYTE_PREFIX   | (codepoint  >> 12));
        dest[1] = (char)(UTF8_CONT_BYTE_MASK | ((codepoint >> 6) & UTF8_CONT_BYTE_BITS));
        dest[2] = (char)(UTF8_CONT_BYTE_MASK | (codepoint & UTF8_CONT_BYTE_BITS));
        return OK;
    } else if (codepoint <= UTF8_4BYTE_MAX) {
        dest[0] = (char)(UTF8_4BYTE_PREFIX   | (codepoint  >> 18));
        dest[1] = (char)(UTF8_CONT_BYTE_MASK | ((codepoint >> 12) & UTF8_CONT_BYTE_BITS));
        dest[2] = (char)(UTF8_CONT_BYTE_MASK | ((codepoint >> 6)  & UTF8_CONT_BYTE_BITS));
        dest[3] = (char)(UTF8_CONT_BYTE_MASK | (codepoint & UTF8_CONT_BYTE_BITS));
        return OK;
    }

return ERR(ERR_INVALID, "invalid codepoint"); 
}

errvt UTF8_streamEncoder(std_Stream* stream, void* data){
	nonull(stream || data, return err);

	std.Stream.Process.writeData(data, strnlen((strc8)data, maxof(len_t)));

return OK;
}

errvt UTF8_streamDecoder(std_Stream* stream, void* data){ 
	
	strc8 str_data = &((strc8)std.Stream.GetPointer(stream))
				[std.Stream.GetCursorPos(stream)];

	iferr(std.String.UTF8.decode(&str_data, data))
		return ERR(ERR_FAIL, "failed to decode stream");

return OK;
}

// UTF-8 to UTF-16 conversion
errvt UTF8_toUtf16(c8* in, len_t in_max, c16* dest, len_t dest_max) {
	nonull(in, return err);
	nonull(dest, return err);

	u64 new_len = 0;
	rune codepoint;

	while (*in || new_len >= dest_max) {
		if (std.String.UTF8.decode(&in, &codepoint) != OK) 
			return ERR(STRINGERR_ENCODING, "Invalid UTF8 sequence");
		
		u64 encoded_len = encode_utf16(&dest[new_len], codepoint);

		if (encoded_len == 0) 
			return ERR(STRINGERR_ENCODING, "Failed to encode UTF16 codepoint");
		
		
		new_len += encoded_len;
	}

return OK;
}


// UTF-8 to UTF-32 conversion
errvt UTF8_toUtf32(c8* in, len_t in_max, c32* dest, len_t dest_max) {
	nonull(in, return err);
	nonull(dest, return err);

	u64 new_len = 0;
	rune codepoint;

	while (*in || new_len >= dest_max) {
		if (std.String.UTF8.decode(&in, &codepoint) != OK) 
			return ERR(STRINGERR_ENCODING, "Invalid UTF8 sequence");
		
		dest[new_len++] = codepoint;
	}

return OK;
}


// UTF-8 to ASCII conversion
errvt UTF8_toAscii(c8* in, len_t in_max, char* dest, len_t dest_max) {
	nonull(in, return err);
	nonull(dest, return err);


	u64 new_len = 0;
	rune codepoint;

	while (*in || new_len >= dest_max) {
		if (std.String.UTF8.decode(&in, &codepoint) != OK) 
			return ERR(STRINGERR_ENCODING, "Invalid UTF8 sequence");
		
		if (codepoint <= ASCII_MAX) {
			dest[new_len++] = (char)codepoint;
		} else {
			// Replace non-ASCII characters with '?'
			dest[new_len++] = ASCII_REPLACEMENT_CHAR;
		}
	}

return OK;
}

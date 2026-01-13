#include <XC.Core/pkg.c>
#include <XC.Core/Essential/Features/String/Encodings/utils.h>
#define module std, String, Encoding, UTF8






len_t moduleFn(len)(c8* str, len_t len, c8** end){
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
			ERR(ERR.INVALID, "invalid utf8 string");
			return 0;
		}
		length++;
	}

	if(end != nil) *end = str;

return length;
}

errvt moduleFn(decode)(char** start, rune* codepoint){

	char* encoding = *start;
	rune c = 0;

	if(!codepoint || codepoint == nil)
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
	    return ERR(ERR.INVALID, "string is an invalid utf8 string");
	}

	*start = encoding;

return OK;
}


// Helper function to encode a UTF-8 codepoint
errvt moduleFn(encode)(char *dest, rune codepoint) {
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

return ERR(ERR.INVALID, "invalid codepoint"); 
}

errvt moduleFn(Encoder)(std_Stream* stream, void* data){
	nonull(stream, data){ return err; }

	std.Stream.Process.writeData(data, strnlen((strc8)data, maxof(len_t)));

return OK;
}

errvt moduleFn(Decoder)(std_Stream* stream, void* data){ 
	
	strc8 str_data = &((strc8)std.Stream.GetPointer(stream))
				[std.Stream.GetCursorPos(stream)];

	iferr(std.String.Encoding.UTF8.decode(&str_data, data))
		return ERR(ERR.FAIL, "failed to decode stream");

return OK;
}

// UTF-8 to UTF-16 conversion
errvt moduleFn(toUtf16)(c8* in, len_t in_max, c16* dest, len_t dest_max) {
	nonull(in, dest){ return err; }

	u64 new_len = 0;
	rune codepoint;

	while (*in || new_len >= dest_max) {
		if (std.String.Encoding.UTF8.decode(&in, &codepoint) != OK) 
			return ERR(ERR.STRING.ENCODING, "Invalid UTF8 sequence");
		
		u64 encoded_len = std.String.Encoding.UTF16.encode(&dest[new_len], codepoint);

		if (encoded_len == 0) 
			return ERR(ERR.STRING.ENCODING, "Failed to encode UTF16 codepoint");
		
		
		new_len += encoded_len;
	}

return OK;
}


// UTF-8 to UTF-32 conversion
errvt moduleFn(toUtf32)(c8* in, len_t in_max, c32* dest, len_t dest_max) {
	nonull(in, dest){ return err; }

	u64 new_len = 0;
	rune codepoint;

	while (*in || new_len >= dest_max) {
		if (std.String.Encoding.UTF8.decode(&in, &codepoint) != OK) 
			return ERR(ERR.STRING.ENCODING, "Invalid UTF8 sequence");
		
		dest[new_len++] = codepoint;
	}

return OK;
}


// UTF-8 to ASCII conversion
errvt moduleFn(toAscii)(c8* in, len_t in_max, char* dest, len_t dest_max) {
	nonull(in, dest){ return err; }


	u64 new_len = 0;
	rune codepoint;

	while (*in || new_len >= dest_max) {
		if (std.String.Encoding.UTF8.decode(&in, &codepoint) != OK) 
			return ERR(ERR.STRING.ENCODING, "Invalid UTF8 sequence");
		
		if (codepoint <= ASCII_MAX) {
			dest[new_len++] = (char)codepoint;
		} else {
			// Replace non-ASCII characters with '?'
			dest[new_len++] = ASCII_REPLACEMENT_CHAR;
		}
	}

return OK;
}



#include <XC.Core/pkg.c>
#include <XC.Core/Essential/Features/String/Encodings/utils.h>

#define module std, String, Utils, Str

len_t moduleFn(len)(std_varData in, len_t len, void** end) {

	DO_LATER("Finish This")

	switchT(&in.type){
	caseT(std_String){


	}
	caseT(strc8){

	}
	caseT(strc16){

	}
	caseT(strc32){

	}
	}

}

len_t moduleFn(siz)(std_varData in, len_t len) {
	void* end = nil;

	std.String.Utils.Str.length(in, len, &end);

return pntr_dist(in, end);
}

bool moduleFn(cmp)(void* str1, void* str2, len_t len){

	char
	    * s1 = str1,
	    * s2 = str2
	;

	len_t len_compared = 0;

	while (*s1 != '\0' && *s2 != '\0' && len_compared < len) {
		// Decode codepoint for s1
		rune codepoint1 = 0, codepoint2 = 0;
		
		if(std.String.Encoding.UTF8.decode(&s1, &codepoint1) != OK &&
		   std.String.Encoding.UTF8.decode(&s2, &codepoint2) != OK
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

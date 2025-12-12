#pragma once
#include "../SIS/v1.h"


#undef label
#define label(name) #name ":"

#undef mov
#define mov(dest, src) "mov" dest", "src

#undef gpr_a
#undef gpr_b
#undef gpr_c
#undef gpr_d

#define gpr_a "eax"
#define gpr_b "ebx"
#define gpr_c "ecx"
#define gpr_d "edx"

#undef  add
#define add(lv, rv) "add" lv", "rv

#undef  jmp
#define jmp(name) "jmp" #name

void test(){
	asm(
	    lbl(SOMETHING)
	    	mov(gpr_a, gpr_c)
	    	add(gpr_a, gpr_c)
	    	jmp(SOMETHING)
	);
}

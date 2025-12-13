#pragma once
#include "../test-utils.h"

bool RUN_ARENA_TESTS(){

NEW_TEST("Arena Allocator Tests"){
	
	inst(Arena) arena = null;
	u32* num_buf = null;

	NEW_SUBTEST("Initialization"){
		if((arena = new(Arena, .init_size = sizeof(u32) * 4)) == null){
			FAIL_TEST
			goto skip;
		}else
			PASS_TEST
	}
	NEW_SUBTEST("Allocation"){
		if((num_buf = Arena.Allocator.New(generic arena, sizeof(u32) * 4, null)) == NULL){
			FAIL_TEST
			goto skip;
		}else
			PASS_TEST
	}
	NEW_SUBTEST("Setting Max"){
		if(Arena.Allocator.Resize(generic arena, null, sizeof(u32) * 4, NULL) != ERR_NONE){
			FAIL_TEST
			goto skip;
		}else
			PASS_TEST
	}



}	
skip:

return TEST_RESULT; 
}

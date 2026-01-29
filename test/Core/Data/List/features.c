#include "../../Test.h"
#define module tests, Data, List, Features

static List(c8) test_list 	= nil;
static List(c8) sublist 	= nil;

alias(std.List, List)


TEST(Initialization){
	
    try() UT
	.newTest("Initialization")
	.assert((test_list = pushList(c8, 10)) != nil);
    catch {
	return err->errorcode;
    }

return OK;
}

TEST(Appending){
    try() UT
	.newTest("Appending")
	.assert(
		List.Append(test_list, "Hello, World!", sizeof("Hello, World!")) == OK
	)
	.equals(
		V(test_list), V("Hello, World!")
	);
    catch {
	printlnErr(
	"Fail Info:\n",
		"\tList Data: ", $(test_list)
	);
	return err->errorcode;	
    }

return OK;
}

TEST(Clearing){

    try() UT
	.newTest("Clearing")
	.assert(({
		List.Flush(test_list); 
		errnm == OK;
	}))
	.assert(len(test_list) == 0);

    catch {
	printlnErr(
	"Fail Info:\n",
		"\tList Data: ", $(test_list)
	);
	return err->errorcode;	
    }

return OK;
}

TEST(Insertion){
	
    try() UT
	.newTest("Insertion")
	.assert(
		List.Insert(test_list, sizeof("Hello, World!"), maxof(len_t), "Hello, World!") == OK
	)
	.equals(
		test_list, "Hello, Other World!"
	)
	.assert(
		List.Insert(test_list , sizeof("Other ") - 1, 7, "Other ") == OK
	)
	.equals(
		test_list, "Hello, Other World!"
	);
    catch {
	printlnErr(
	"Fail Info:\n",
		"\tList Data: ", $(test_list)
	);
	return err->errorcode;	
    }

return OK;
}

TEST(Sublist){
	
    try() UT
	.newTest("Sublist")
	.assert((sublist = List.SubList(test_list, 7, sizeof("Other ") - 1)) != nil)
	.equals(sublist, "Other ")
		;
		TestListNotFor(sublist, "Other "){
			FAIL_TEST
			loginfo("FAIL RESULT: ",$((strc8)List.GetPointer(sublist,0)));
			goto skip;
		}
		List.Flush(sublist);
		//Tests both:
			//sublisting the entire test_list from index,
			//freeing and reinitialization of an already created test_list
		sublist = List.SubList(test_list, 7, UINT64_MAX);
		TestListNotFor(sublist, "Other World!"){
			FAIL_TEST
			loginfo("FAIL RESULT: ",$((strc8)List.GetPointer(sublist,0)));
			goto skip;
		}else 
			PASS_TEST
	}

return OK;
}


#undef module
#define module tests, Data, List

Tests moduleFn(Features)(){
	static Tests tests = {};

	if(!tests.unit)
	tests.unit = new(UnitTest, 
		.name 		= "XC.Core.Data.List",
		.procedures 	= arr(
			&mod(Features_Initialization),
			&mod(Features_Appending),
			&mod(Features_Clearing),
			&mod(Features_Insertion),
		),
	);

return tests;


}

TEST(LIST_FEATURES){

	NEW_SUBTEST("Merging"){
		List.Flush(test_list);
		List.Insert(test_list, sizeof("Goodbye,  See you tommorrow!"),
	     		UINT64_MAX, "Goodbye,  See you tommorrow!");
		List.Pop(sublist, 1);// removing the null terminator
		List.Merge(test_list, sublist, 9);
		TestListNotFor(test_list, "Goodbye, Other World! See you tommorrow!"){
			FAIL_TEST
			loginfo("FAIL RESULT: ",$((strc8)List.GetPointer(test_list,0)));
			goto skip;
		}else 
			PASS_TEST
	}

	NEW_SUBTEST("Iteration"){
		foreach(test_list, char, c){
		    if("Goodbye, Other World! See you tommorrow!"[c_iterator] != c){
			FAIL_TEST
			loginfo("FAIL RESULT: ",$((strc8)List.GetPointer(test_list,0)));
			goto skip;
		    }
		}
	}

	NEW_SUBTEST("Free slots"){
		loop(i, sizeof("Goodbye") - 1)
		    iferr(List.SetFree(test_list, i)){
			FAIL_TEST
			loginfo("FAIL RESULT: ",$((strc8)List.GetPointer(test_list,0)));
			goto skip;
		    }
	}

	NEW_SUBTEST("Fill slots"){
		loop(i, sizeof("Goodbye") - 1)
		    List.FillSlot(test_list, &("Goodbye"[i]));

		TestListNotFor(test_list, "eybdooG, Other World! See you tommorrow!"){
			FAIL_TEST
			loginfo("FAIL RESULT: ",$((strc8)List.GetPointer(test_list,0)));
			goto skip;
		}
	}

	NEW_SUBTEST("Casting"){
	    quiet(){
		ListCast(sublist, u32);
		
		if(List.Merge(test_list, sublist, 9) != DATAERR_SIZETOOLARGE){
			FAIL_TEST
			loginfo("FAIL RESULT: Incorrect error code returned");
			goto skip;
		}
		
		TestListNotFor(test_list, "Goodbye, Other World! See you tommorrow!"){
			FAIL_TEST
			loginfo("FAIL RESULT: ",$((char*)List.GetPointer(test_list,0)));
			goto skip;
		}else 
			PASS_TEST
	    }
	}
skip:

return TEST_RESULT; 
}

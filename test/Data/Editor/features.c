#include "../../Test.h"
#define module tests, Data, List, Features

static List(c8) test_list 	= nil;
static List(c8) sublist 	= nil;

alias(std.List, List)

#define equals(string)	\
	assert(strncmp(index(test_list, 0), string, sizeof(string) - 1))

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
	.equals("Hello, World!");
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
	.equals("Hello, World!")
	.assert(
		List.Insert(test_list , sizeof("Other ") - 1, 7, "Other ") == OK
	)
	.equals("Hello, Other World!");
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
	.assert(
		(sublist = List.SubList(test_list, 7, sizeof("Other ") - 1)) != nil
	)
	.equals("Other ")
	.assert(({
		List.Flush(sublist);

		sublist = List.SubList(test_list, 7, maxof(len_t));
		sublist != nil;
	}))
	.equals("Other World!");
    catch {
	printlnErr(
	"Fail Info:\n",
		"\tList Data: ", $(test_list)
	);
	return err->errorcode;	
    }
	

return OK;
}

TEST(Merging){

    try() UT
	.newTest("Merging")
	.assert(({
		List.Flush(test_list);
		List.Insert(
			test_list, 
			sizeof("Goodbye,  See you tommorrow!"),
	 		maxof(len_t),
			"Goodbye,  See you tommorrow!"
	 	);
		List.Pop(sublist, 1);// removing the null terminator
		List.Merge(test_list, sublist, 9);
	}))
	.equals("Goodbye, Other World! See you tommorrow!");
    catch {
	printlnErr(
	"Fail Info:\n",
		"\tList Data: ", $(test_list)
	);
	return err->errorcode;	

    }

return OK;
}

TEST(Iteration){

    try() UT
	.newTest("Iteration")
	.assert(({
		bool fail = false;
		foreach(test_list, char, c){
		    if("Goodbye, Other World! See you tommorrow!"[c_iterator] != *c){
			fail = true;
			break;
		    }
		}
		fail;
	}));
    catch {
	printlnErr(
	"Fail Info:\n",
		"\tList Data: ", $(test_list)
	);
	return err->errorcode;	
    }

return OK;
}
TEST(SetFree){

    try() UT
	.newTest("SetFree")
	.assert(({
		bool passed = true;
		loop(i, sizeof("Goodbye") - 1)
		    iferr(List.SetFree(test_list, i)){
			passed = false;
			break;
		    }
		passed;
	}));
    catch {
	printlnErr(
	"Fail Info:\n",
		"\tList Data: ", $(test_list)
	);
	return err->errorcode;	
    }

return OK;
}

TEST(FillSlot){

    try() UT
	.newTest("FillSlot")
	.run(({
		loop(i, sizeof("Goodbye") - 1)
		    List.FillSlot(test_list, &("Goodbye"[i]));

		run_continue;
	}))
	.equals("eybdooG, Other World! See you tommorrow!");
    catch {
	printlnErr(
	"Fail Info:\n",
		"\tList Data: ", $(test_list)
	);
	return err->errorcode;	
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
			&mod(Features_Sublist),
			&mod(Features_Merging),
			&mod(Features_Iteration),
			&mod(Features_SetFree),
			&mod(Features_FillSlot)
		),
	);

return tests;


}

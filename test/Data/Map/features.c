#include "../../Test.h"
#define module tests, Data, Map, Features

Map(std_String, u32) map = nil;
List(data_entry) entries = nil;

u32 		data[3] = {31243552, 4509302, 391204802};
strc8 		keys[3] = {"hi", "Lorem Ispbum", "Hello World!"};
u32 		indexes[3] = {INVALID_MAPINDEX, INVALID_MAPINDEX, INVALID_MAPINDEX};

TEST(Initialization){
    try() UT
	.newTest("Initialization")
	.assert(({
		map = newMap(std_String, u32, 5);
		map != nil;
	}));
    catch { return err->errorcode; }

return OK;
}

TEST(GetEntries){
    try() UT
	.newTest("GetEntries")
	.assert(({
		entries = std.Map.GetEntries(map);

		entries != nil && len(entries) == 0;
	}));
    catch { return err->errorcode; }

return OK;
}

TEST(Insert){
    try() UT
	.newTest("Insert")
	.assert(({
		indexes[0] = std.Map.Insert(map, asString(keys[0], 100), &data[0]);
		indexes[1] = std.Map.Insert(map, asString(keys[1], 100), &data[1]);
		indexes[2] = std.Map.Insert(map, asString(keys[2], 100), &data[2]);

		len(entries) == 3 		&& 
		indexes[0] != INVALID_MAPINDEX	&&
		indexes[1] != INVALID_MAPINDEX	&&
		indexes[2] != INVALID_MAPINDEX;
	}));
    catch { return err->errorcode; }

return OK;
}

TEST(SearchIndex){

    try() UT
	.newTest("SearchIndex")
	.assert(({
		u32 key_index = INVALID_MAPINDEX;
		bool passed = true;

		loop(i, 3)
		    passed = ({
			key_index = std.Map.SearchIndex(map, asString(keys[i], 100));
			key_index != INVALID_MAPINDEX && key_index == indexes[i];
		    });

		passed;
	}));
    catch { return err->errorcode; }

return OK;
}

TEST(Index){

    try() UT
	.newTest("Index")
	.assert(({
		u32* index_result = nil;
		bool passed = true;

		loop(i, 3)
		    passed = ({
			index_result = std.Map.Index(map, indexes[i]);
			index_result != nil && *index_result == data[i];
		    });

		passed;
	}));
    catch { return err->errorcode; }

return OK;
}

TEST(Search){

    try() UT
	.newTest("Search")
	.assert(({
		u32* index_result = nil;
		bool passed = true;

		loop(i, 3)
		    passed = ({
			index_result = std.Map.Search(map, asString(keys[i], 100));
			index_result != nil && *index_result == data[i];
		    });

		passed;
	}));
    catch { return err->errorcode; }

return OK;
}


TEST(Remove){

    try() UT
	.newTest("Remove")
	.assert(({
		len_t num_expected_entries = 3;
		bool passed = true;
		loop(i, 3)
		    passed = ({
			std.Map.Remove(map, asString(keys[i], 100)) == OK &&
			len(entries) == --num_expected_entries;
		    });

		passed;
	}));
    catch { return err->errorcode; }

return OK;
}

#undef module
#define module tests, Data, Map

Tests moduleFn(Features)(){
	static Tests tests = {};

	if(!tests.unit)
	tests.unit = new(UnitTest, 
		.name 		= "XC.Core.Data.Map",
		.procedures 	= arr(
		  	&mod(Features_Initialization),
		  	&mod(Features_GetEntries),
		  	&mod(Features_Insert),
		  	&mod(Features_SearchIndex),
		  	&mod(Features_Index),
		  	&mod(Features_Search),
		  	&mod(Features_Remove)
		),
	);

return tests;
}

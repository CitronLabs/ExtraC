#include "../../Test.h"
#define module tests, Data, DSN, Features	

alias(std.Map.Search, 		  search);
alias(std.Number.Compare, 	  numCompare);
alias(std.Number.Equality.EQUALS, EQUALS);

static std_DSN* dsn;

TEST(Initialization){
    try() UT
	.newTest("Initialization")
	.assert((dsn = push(std_DSN)) != nil);
    catch {
	return err->errorcode;
    }

return OK;
}

TEST(Parse_List){
	std_List* list 		= nil;
	len_t     parsed_len 	= 0;
	var       inputStream 	= push(std_Stream);

    try() UT
	.newTest("Parse_List")
	.assert(({
		printTo(inputStream, "[1, 2, 3]"); 

		var parsed_len = std.DSN.List.parse(dsn, &list, inputStream);

			
		parsed_len != 0 && 
		len(list) == 3  &&
		numCompare(index(list, 0), n(1)) == EQUALS &&
		numCompare(index(list, 1), n(2)) == EQUALS &&
		numCompare(index(list, 2), n(3)) == EQUALS;
	}));
    catch {
	printlnErr(
	"Fail Info:\n"
		"\tInput Stream: ",  $(inputStream), ",\n",
		"\tParsed Length: ", $(parsed_len),  ",\n",
		"\tList Result: ",   $(list)
	);

	del(list);
	pop(inputStream);
	return err->errorcode;
    }
	del(list);
	pop(inputStream);

return OK;
}


TEST(Parse_Map){
	std_Map*  map 		= nil;
	len_t     parsed_len 	= 0;
	var       inputStream 	= push(std_Stream);

    try() UT
	.newTest("Parse_Map")
	.assert(({
		printTo(inputStream, "@{\"key1\"->1,\"key2\"->2}"); 

		var parsed_len = std.DSN.Map.parse(dsn, &map, inputStream);

		parsed_len != 0 && 
		len(map) == 2   &&
		numCompare(search(map, "key1"), n(1)) == EQUALS  &&
		numCompare(search(map, "key2"), n(2)) == EQUALS;
	}));
    catch {
	printlnErr(
	"Fail Info:\n"
		"\tInput Stream: ",  $(inputStream), ",\n",
		"\tParsed Length: ", $(parsed_len),  ",\n",
		"\tMap Result: ",    $(map)
	);

	del(map);
	pop(inputStream);
	return err->errorcode;
    }
	del(map);
	pop(inputStream);

return OK;
}

TEST(Parse_Struct){
	std_Struct* struct_data	= nil;
	len_t       parsed_len 	= 0;
	var         inputStream = push(std_Stream);

    try() UT
	.newTest("Parse_Map")
	.assert(({
		printTo(inputStream, "@{\"key1\"->1,\"key2\"->2}"); 

		var parsed_len = std.DSN.Struct.parse(dsn, &struct_data, inputStream);
			
		parsed_len != 0 && len(struct_data) == 3;
	}));
    catch {
	printlnErr(
	"Fail Info:\n"
		"\tInput Stream: ",  $(inputStream), ",\n",
		"\tParsed Length: ", $(parsed_len),  ",\n",
		"\tStruct Result: ", $(struct_data)
	);

	del(struct_data);
	pop(inputStream);
	return err->errorcode;
    }
	del(struct_data);
	pop(inputStream);

return OK;
}

TEST(Parse_DSN_NoImports){
	const c8 input[] = {
		#embed "Assets/Person.dsn"
	};


return OK;
}
TEST(Parse_DSN_WithImports){
	const c8 input[] = {
		#embed "Assets/Order.dsn"
	};
	const c8 import[] = {
		#embed "Assets/Menu.dsn"
	};

	var         inputStream = push(std_Stream);
	std_DSN*    importDSN   = push(std_DSN);

    try() UT
	.newTest("Parse_Map")
	.assert(({
		printTo(inputStream, import); 

		var parsed_len = std.DSN.parse(dsn, nil, inputStream);
			
		parsed_len != 0 && len(dsn->body);
	}));
    catch {
	printlnErr(
	"Fail Info:\n"
		"\tInput Stream: ",  $(inputStream), ",\n",
		"\tParsed Length: ", $(parsed_len),  ",\n",
		"\tStruct Result: ", $(struct_data)
	);

	del(struct_data);
	pop(inputStream);
	return err->errorcode;
    }
	del(struct_data);
	pop(inputStream);

return OK;

return OK;
}
#undef module
#define module tests, Data, DSN

Tests moduleFn(Features)(){
	static Tests tests = {};

	if(!tests.unit)
	tests.unit = new(UnitTest, 
		.name 		= "XC.Core.Data.DSN",
		.procedures 	= arr(
		  	&mod(Features_Initialization),
		  	&mod(Features_Parse_List),
		  	&mod(Features_Parse_Map),
		  	&mod(Features_Parse_Struct),
		  	&mod(Features_Parse_DSN_NoImports),
		  	&mod(Features_Parse_DSN_WithImports),
		),
	);

return tests;


}

TEST(DSN_FEATURES_PARSING);
TEST(DSN_FEATURES_PRINTING);
TEST(DSN_FEATURES_IMPORTING);

TEST(DSN_FEATURES){

	dsn_var = new(DSN);

	RUN_TEST(DSN_FEATURES_PRINTING){
		return TEST_RESULT;
	}

	RUN_TEST(DSN_FEATURES_PARSING){
		return TEST_RESULT;
	}

	RUN_TEST(DSN_FEATURES_IMPORTING){
		return TEST_RESULT;
	}
	
	del(dsn_var);

return TEST_RESULT;	
}


TEST(DSN_FEATURES_PARSING){
	
	NEW_SUBTEST("Parse Struct"){
		inst(String) struct_str = s("{field1 = 1, field2 = \"string\"}");
		parsed_len = DSN.parseStruct(dsn_var, (inst(Struct)*)&parsed_data.data, struct_str);
		if (parsed_len > 0 && parsed_data.type == DSN_STRUCT && Struct.SearchField((inst(Struct))parsed_data.data, s("field1")) != null) {
		    PASS_TEST
		} else {
		    FAIL_TEST
		}
		pop((inst(Struct))parsed_data.data);
	}

return TEST_RESULT; 


}
TEST(DSN_FEATURES_PRINTING){
	inst(StringBuilder) sb = new(StringBuilder);
    
	NEW_SUBTEST("Format List"){
		List(int) list = pushList(int, 10);
		List.Append(list, (int[]){1,2,3}, 3);
		DSN.formatList(dsn_var, list, sb);
		if (strncmp(StringBuilder.GetStr(sb).txt.data.utf8, "[ 1, 2, 3 ]", 11) == 0) {
		    PASS_TEST
		} else {
		    FAIL_TEST
		}
		StringBuilder.Clear(sb);
		pop(list);
	}
	NEW_SUBTEST("Format Map"){
		Map(String, int) map = pushMap(String, int);
		int val1 = 1, val2 = 2;
		Map.Insert(map, s("key1"), &val1);
		Map.Insert(map, s("key2"), &val2);
		DSN.formatMap(dsn_var, map, sb);
		if (strncmp(StringBuilder.GetStr(sb).txt.data.utf8, "@{ \"key1\"->1, \"key2\"->2, }", 23) == 0) {
		    PASS_TEST
		} else {
		    FAIL_TEST
		}
		StringBuilder.Clear(sb);
		pop(map);
	}
	pop(sb);

}
TEST(DSN_FEATURES_IMPORTING){


}

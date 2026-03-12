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

TEST(Parse_String){
	std_String* string 	= nil;
	len_t       parsed_len 	= 0;
	var         inputStream = push(std_Stream);

    try() UT
	.newTest("Parse_String")
	.assert(({
		printTo(inputStream, "\"Hello\""); 

		parsed_len = std.DSN.String.parse(dsn, &string, inputStream);
			
		parsed_len  != 0  && 
		len(string) == 5  &&
		std.String.Compare(string, s("Hello"));
	}));
    catch {
	printlnErr(
	"Fail Info:\n"
		"\tInput Stream: ",  $(inputStream), ",\n",
		"\tParsed Length: ", $(parsed_len),  ",\n",
		"\tString Result: ", $(string)
	);

	del(string);
	pop(inputStream);
	return err->errorcode;
    }
	del(string);
	pop(inputStream);

return OK;
}

TEST(Parse_Number){
	std_Number* number 	= nil;
	len_t       parsed_len 	= 0;
	var         inputStream = push(std_Stream);

    try() UT
	.newTest("Parse_Number")
	.assert(({
		printTo(inputStream, "100"); 

		parsed_len = std.DSN.Number.parse(dsn, &number, inputStream);
			
		parsed_len  != 0  && 
		len(number) == 5  &&
		numCompare(number, n(100));
	}));
    catch {
	printlnErr(
	"Fail Info:\n"
		"\tInput Stream: ",  $(inputStream), ",\n",
		"\tParsed Length: ", $(parsed_len),  ",\n",
		"\tNumber Result: ", $(number)
	);

	del(number);
	pop(inputStream);
	return err->errorcode;
    }
	del(number);
	pop(inputStream);

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

		parsed_len = std.DSN.List.parse(dsn, &list, inputStream);

			
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

		parsed_len = std.DSN.Map.parse(dsn, &map, inputStream);

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
	.newTest("Parse_Struct")
	.assert(({
		printTo(inputStream, 
	  		"{"
	  		    "number =  10,"
	  		    "string = \"Hello\","
	  		    "list   =  [1,2,3],"
	  		    "map    =  @{1->\"Hey\"},"
	  		    "struct =  {number = 15}"
			"}"); 

		parsed_len = std.DSN.Struct.parse(dsn, &struct_data, inputStream);
			
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
		#embed "Assets/Person.dsn" suffix(, 0)
	};

	var      inputStream = push(std_Stream);
	len_t 	 parsed_len  = 0;

    try() UT
	.newTest("Parse_DSN_NoImports")
	.assert(({
		printTo(inputStream, input); 

		parsed_len = std.DSN.parse(dsn, nil, inputStream);
			
		parsed_len != 0 && len(dsn->body) &&
		std.String.Compare(
			std.DSN.search(dsn, s("Name"))->asString, 
			s("John Smith")
		) &&
		numCompare(
			std.DSN.search(dsn, s("Age"))->asNumber,
			n(38)
		) == EQUALS;
	}));
    catch {
	printlnErr(
	"Fail Info:\n"
		"\tInput Text: ",    input, 	     ",\n",
		"\tInput Stream: ",  $(inputStream), ",\n",
		"\tParsed Length: ", $(parsed_len),  ",\n",
		"\tInput Result: ",  $(dsn)
	);

	del(dsn);
	pop(inputStream);
	return err->errorcode;
    }

return OK;
}
TEST(Parse_DSN_WithImports){
	const c8 input[] = {
		#embed "Assets/Order.dsn" suffix(, 0)
	};
	const c8 import[] = {
		#embed "Assets/Menu.dsn" suffix(, 0)
	};

	var      inputStream = push(std_Stream);
	len_t 	 parsed_len  = 0;
	std_DSN* importDSN   = push(std_DSN);

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
		"\tInput Text: ",    input, 	     ",\n",
		"\tImport Text: ",   import, 	     ",\n",
		"\tInput Stream: ",  $(inputStream), ",\n",
		"\tParsed Length: ", $(parsed_len),  ",\n",
		"\tImport Result: ", $(importDSN),   ",\n",
		"\tInput Result: ",  $(dsn)
	);

	del(dsn);
	pop(inputStream, importDSN);
	return err->errorcode;
    }
	del(dsn);
	pop(inputStream, importDSN);

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
		  	&mod(Features_Parse_String),
		  	&mod(Features_Parse_Number),
		  	&mod(Features_Parse_List),
		  	&mod(Features_Parse_Map),
		  	&mod(Features_Parse_Struct),
		  	&mod(Features_Parse_DSN_NoImports),
		  	&mod(Features_Parse_DSN_WithImports),
		),
	);

return tests;
}


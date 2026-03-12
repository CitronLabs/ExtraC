#define IMPL_STRING
#include "Editor.h"

thread_local struct {
	Stack buffers;
	std_List* current;
} Process;

ArrayStack(std_List) moduleFn(getBufferStack)(){
	
	if(!Process.buffers.__type){
	    if(!createArrayStack(List, &Process.buffers, 5)){
		ERR(ERR.INIT, "Failed to initialize string buffers stack");
		return nil;
	    }
	}

return &Process.buffers;
}

StrProc moduleFn(start)(std_String* self){
	var   bufferStack = mod(getBufferStack)();

	iferr(std.Array.Reserve(bufferStack, std.Array.Grow.ATLEAST, 1)){
		ERR(ERR.FAIL, "Failed to reserve buffer object");
		return STOP;
	}

	List* buffer = index(bufferStack, len(bufferStack));

	try() buffer = createList(byte, buffer,	strsize(self, len(self))); catch {
		ERR(ERR.FAIL, "Failed to initialize string buffer");
		return STOP;
	}

	if(!write(mod(getBufferStack)(), buffer)){
		ERR(ERR.FAIL, "Failed to write to string buffer");
		return STOP;
	}

	Process.current =  buffer;

return CONTINUE;
}

noFail moduleFn(end)(){
	pop(Process.current);

	iferr(std.Array.Read(mod(getBufferStack)(), nil, 1)){
		ERR(ERR.FAIL, "Failed to pop last string buffer from stack");
	}

return;
}
noFail moduleFn(fail)(){ mod(end)(); }

std_String* moduleFn(result)(){ return mod(getStr)(); }

StrProc moduleFn(run)(bool cont){ return cont ? CONTINUE : STOP; }


StrProc moduleFn(clear)(){ std.List.Flush(Process.current); return CONTINUE; }

StrProc moduleFn(strip)(){
	std_String* string     = mod(getStr)();
	var 	    indexStack = pushArrayStack(len_t, 20);	

	foreach(string, rune, c)
	    if(isblank(*c)) write(indexStack, &c_iterator);
	    
	foreach(indexStack, len_t, index)
		std.List.Remove(Process.current, *index, 1);

	pop(indexStack);

return CONTINUE;
}

StrProc moduleFn(upper)(){
	std_String* string     = mod(getStr)();

	foreach(string, rune, c)
	    if(islower(*c)){
		var new_c = toupper(*c);
		UTF32.toUtf8(&new_c, 1, index(string, c_iterator), 1);
	    }

return CONTINUE;
}

StrProc moduleFn(lower)(){
	std_String* string     = mod(getStr)();

	foreach(string, rune, c)
	    if(isupper(*c)) {
		var new_c = tolower(*c);
		UTF32.toUtf8(c, 1, index(string, c_iterator), 1);
	    }

return CONTINUE;
}

StrProc moduleFn(reverse)(){
	var tempStack = pushArrayStack(rune, size(Process.current));
	var string    = mod(getStr)();

	foreach(string, rune, c)
	    write(tempStack, c);
	
	iferr(std.String.Encoding.UTF32.toUtf8(
		index(tempStack, 0), len(tempStack), 
		index(Process.current, 0), len(Process.current)
	)){
	    ERR(ERR.FAIL, "Failed to reverse string");
	    return STOP;
	}

return CONTINUE;
}

StrProc moduleFn(padLeft)(len_t len){
	var padList = newList(byte, len + size(Process.current));

	loop(i, len) write(padList, " ");
	
	iferr(std.List.Merge(padList, Process.current, maxof(len_t))){
		del(padList);
		ERR(ERR.FAIL, "Failed to add padding to string");
		return STOP;
	}

	pop(Process.current);

	Process.current = padList;

return CONTINUE;
}

StrProc moduleFn(append)(std_String* string){
	var originalStr = string;

	if(TFrom(string).id != T(std_String_UTF8).id){

	    string = copy(string, push(std_String));

	    if(!string){
		ERR(ERR.FAIL, "Failed to copy input string for converting");
		return STOP;
	    }
	   
	   iferr(std.String.Encoding.Convert(string, std.String.Encoding.Type.UTF8)){
		ERR(ERR.FAIL, "Failed to convert input string for appending");
		return STOP;
	   }
	}

	iferr(std.List.Append(
		Process.current, 
		string->data, 
		strsize(string, string->len)
	)){
		ERR(ERR.FAIL, "Failed to append text to string");
		return STOP;
	}

	if(string != originalStr) pop(string);

return CONTINUE;
}

StrProc moduleFn(prepend)(std_String* string){
	var originalStr = string;

	if(TFrom(string).id != T(std_String_UTF8).id){

	    string = copy(string, push(std_String));

	    if(!string){
		ERR(ERR.FAIL, "Failed to copy input string for converting");
		return STOP;
	    }
	   
	   iferr(std.String.Encoding.Convert(string, std.String.Encoding.Type.UTF8)){
		ERR(ERR.FAIL, "Failed to convert input string for appending");
		return STOP;
	   }
	}

	iferr(std.List.Insert(
		Process.current, 
		strsize(string, string->len),
		maxof(len_t),
		string->data 
	)){
		ERR(ERR.FAIL, "Failed to append text to string");
		return STOP;
	}

	if(string != originalStr) pop(string);

return CONTINUE;
}

StrProc moduleFn(insert)(len_t index, std_String* string){

	var originalStr = string;

	if(TFrom(string).id != T(std_String_UTF8).id){

	    string = copy(string, push(std_String));

	    if(!string){
		ERR(ERR.FAIL, "Failed to copy input string for converting");
		return STOP;
	    }
	   
	   iferr(std.String.Encoding.Convert(string, std.String.Encoding.Type.UTF8)){
		ERR(ERR.FAIL, "Failed to convert input string for appending");
		return STOP;
	   }
	}

	iferr(std.List.Insert(
		Process.current, 
		strsize(string, string->len),
		index,
		string->data 
	)){
		ERR(ERR.FAIL, "Failed to append text to string");
		return STOP;
	}

	if(string != originalStr) pop(string);

return CONTINUE;
}


StrProc moduleFn(slice)(len_t top, len_t bottom){
	if(top > bottom){
		ERR(ERR.FAIL, "top of slice is clashes with bottom of slice");
		return STOP;
	}

	var originalLen = len(Process.current);

	var newBuffer = std.List.SubList(
		Process.current, 
		new(List), 
		top, 
		originalLen - (top + bottom)
	);

	if(!newBuffer){
		ERR(ERR.FAIL, "Failed to create slice original string");
		return STOP;
	}

	del(Process.current);

	Process.current = newBuffer;

return CONTINUE;
}

StrProc moduleFn(replace)(len_t startsAt, len_t num, std_String* search, std_String* replace){


}

StrProc moduleFn(each)(rune* fn()(rune* character, pntr info), pntr info);

std_String* moduleFn(getStr)();
std_String* moduleFn(makeStr)();

Array(len_t) moduleFn(find)(std_String* string, len_t num);


bool moduleFn(contains)(std_String*);
bool moduleFn(starts)(std_String*);
bool moduleFn(ends)(std_String*);

#undef module

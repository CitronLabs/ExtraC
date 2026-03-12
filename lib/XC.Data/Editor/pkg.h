#pragma once

#define __XC_DATA__ 
#include "../pkg.h"
#include "../List/pkg.h"
#include "../String/pkg.h"

//=======EXPORTS========
#ifndef __XC_EDITOR__
#define package std

Interface(Editor,
	Process(String, std_String,
	 	fn(clear)(),
		fn(strip)(),
		fn(upper)(),
		fn(lower)(),
	 	fn(reverse)(),
	 	fn(padLeft)(len_t),
		fn(append)(std_String*),
	 	fn(prepend)(std_String*),
	 	fn(insert)(len_t, std_String*),
	 	fn(slice)(len_t top, len_t bottom),
		fn(replace)(
	 		len_t startsAt, 
	 		len_t num,
	 		std_String* search, 
	 		std_String* replace 
	 	),
	 	fn(each)(rune* fn()(rune* character, pntr info), pntr info);
		
		std_String
			* fn(getStr)(),
			* fn(makeStr)();

	 	Array(len_t) fn(find)(std_String*, len_t);
	 	bool fn(contains)(std_String*);
	 	bool fn(starts)(std_String*);
	 	bool fn(ends)(std_String*);


	)
	Process(Bytes, pntr,
		fn(setLen)(len_t),
		fn(reverse)(),
	 	fn(flipEndian)(),
	 	fn(shiftLeft)(len_t),
	 	fn(shiftRight)(len_t),
	 	fn(bitXor)(),
	 	fn(bitAnd)(),
	 	fn(bitOr)(),
	 	fn(bitNot)(),
	 	fn(mask)(),
	 	fn(overwrite)(),
	 	fn(fill)();

		word fn(compare)(byte* other, len_t len);
	 	Array(len_t) fn(find)(byte* sequence, len_t len);

	)
	Process(Array, std_Array,
		fn(push)(pntr),
		fn(pop)(pntr),
	 	fn(insert)(pntr, len_t),
	 	fn(remove)(len_t),
	 	fn(reverse)(),
	 	fn(sort)(    bool fn()(pntr a, pntr b, pntr info), pntr info),
	 	fn(each)(    pntr fn()(pntr val, pntr info), pntr info),
	 	fn(filter)(  bool fn()(pntr val, pntr info), pntr info);

	 	std_Array_Buffer* fn(makeBuffer)();
	 	std_Array_Stack* fn(makeStack)();
	 	std_Array_Queue* fn(makeQueue)();
	 	std_Array_List* fn(makeList)();

	 	pntr 
	 	    fn(grab)(len_t index),
	 	    fn(search)(
	 		len_t fn(search)(pntr, len_t, pntr),
	 		pntr  val
	 	    ),
		    fn(last)(),
		    fn(first)(),
	 	    fn(middle)(bool roundUp);

	)
)

#undef package
#endif

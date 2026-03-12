#include <XC.pkg.h>
#define module std, Editor

exportFrom(String,
SUBMODULE(),
VALUES(),
	start, end, run, fail, result,

	clear, strip, upper, lower, reverse,
	padLeft, append, prepend, insert,
	slice, replace, each, getStr, makeStr,
	find, contains, starts, ends
)

exportFrom(Bytes,
SUBMODULE(),
VALUES(),
	start, end, run, fail, result,

	setLen,	reverse, flipEndian, shiftLeft,
	shiftRight, bitXor, bitAnd, bitOr, bitNot,
	mask, overwrite, fill, compare,	find
)

exportFrom(Array,
SUBMODULE(),
VALUES(),
	start, end, run, fail, result,

	push, pop, insert, remove, reverse,
	sort, each, filter, makeBuffer,	makeStack,
	makeQueue, makeList, grab, search, last, first,
	middle
)

export(
SUBMODULE(String, Bytes, Array),
VALUES(),
)

#undef module

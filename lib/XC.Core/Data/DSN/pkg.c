#include <XC.Core/pkg.h>

#define module std, DSN

moduleValues(Field,
	NONE,,
	LIST,,
	MAP,,
	STRUCT,,
	NUMBER,,
	STRING,,
);

exportFrom(Export,
SUBMODULE(), 
VALUES(), 
	JSON
)

exportFrom(Import,
SUBMODULE(), 
VALUES(),
	JSON
)

exportFrom(List,
SUBMODULE(), 
VALUES(), 
	format,  parse, 
	encoder, decoder
)

exportFrom(Map,
SUBMODULE(), 
VALUES(), 
	format,  parse, 
	encoder, decoder
)

exportFrom(Struct,
SUBMODULE(), 
VALUES(), 
	format,  parse, 
	encoder, decoder
)

exportFrom(String,
SUBMODULE(), 
VALUES(), 
	format,  parse, 
	encoder, decoder
)

exportFrom(Number,
SUBMODULE(), 
VALUES(), 
	format,  parse, 
	encoder, decoder
)


exportFrom(DSB,
SUBMODULE(), 
VALUES(), 
	format,  parse, 
	encoder, decoder
)

export(
SUBMODULE(
	Export, Import, 
	List,   Map,
	Struct, String,
	Number, DSB
), 
VALUES(Field), 
	search,  include,
	parse,   format,
	decoder, encoder

);

#undef module

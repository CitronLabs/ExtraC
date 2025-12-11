#include <Core/pkg.h>

#define module std, DSN

moduleValues(Field,
	NONE,,
	LIST,,
	MAP,,
	STRUCT,,
	NUMBER,,
	STRING,,
);



export(
SUBMODULE(), 
VALUES(Field), 
	search
);

#undef module

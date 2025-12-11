#include <Core/pkg.c>

#define module std, Object

construct(std_Object,
FMT(),
DEF(),
	.Scan    = nil,
	.Print   = nil,
	.Hash    = nil,
	.Size    = nil,
	.Copy    = nil,
	.Create  = nil,
	.Destroy = nil,
	.Set	 = nil,
	.Iter 	 = nil,
	.Read 	 = nil,
	.Write   = nil,
);



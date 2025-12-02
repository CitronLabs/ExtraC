#include "../../include/resource.h"

private(Collection,
	inst(MemoryMap) map;
	List(memid) resources;
)



u32 methodimpl(Collection, numOfResources){
	return List.Size(priv.resources);
	
}

Constructor(Collection,
__METHODS(
	.numOfResources = Collection_numOfResources
),
	*priv = (Collection_Private){
		.map = arg.memMap,
		.resources = newList(memid, 10)
	};

	if(arg.memMap == null)
		failinit(ERR_NULLPTR, "memory map cannot be null");
	
	if(priv.resources == null)
		failinit(ERR_INITFAIL, "failed to create resource list");
	
)

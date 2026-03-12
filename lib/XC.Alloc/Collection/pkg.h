
Class(Collection,
INIT(inst(MemoryMap) memMap; memid parent;),
FIELD(bool warnRemoval),
	
	#define collect(Collection, Class, ...) Class.__COLLECT(Collection, (Class##_ConstructArgs){__VA_ARGS__})

	u32 method(Collection, numOfResources);

)

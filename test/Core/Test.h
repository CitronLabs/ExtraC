#include <XC.Builtin/pkg.c>

Module(tests){
  submodule(Alloc,
      Tests 
            fn(Buffer)(),
            fn(Arena)(),
            fn(Pool)();
  )
  submodule(Data,
      Tests 
            fn(List)(),
            fn(Map)(),
            fn(Struct)(),
            fn(DSN)();
  )
  submodule(Debug,
      Tests 
            fn(Logger)();
  )
  submodule(Math,
      Tests 
            fn(Number)();
  )
  submodule(Strings,
      Tests 
            fn(Editor)();
  )
  submodule(Time,
      Tests 
            fn(Clock)(),
            fn(Date)(),
            fn(Watch)(),
            fn(Time)();
  )
  submodule(Features,
      Tests 
            fn(Type)(),
            fn(Memory)(),
            fn(Stream)(),
            fn(Process)(),
            fn(Arrays)(),
            fn(Error)(),
            fn(String)();
  )

	values(Type, u32, 
		UNIT
	)

	Array() fn(getAll)(u32 type);
};

#undef package
#define module tests

exportFrom(Data
SUBMODULE(), 
VALUES(), 
)
export(
SUBMODULE(), 
VALUES(), 
)


#undef module

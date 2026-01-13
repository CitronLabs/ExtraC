#include <XC.Builtin/pkg.c>


Module(tests){
  submodule(Core,
    submodule(Dev,
	Tests 
	      fn(Stream)(),
	      fn(Register)(),
	      fn(Device)();
    )
    submodule(Sys,
	Tests 
	      fn(Sys)(),
	      fn(Time)(),
	      fn(Mem)(),
	      fn(Arch)(),
	      fn(Locale)();
    )
  )
};
#undef package
#define module tests

export(
SUBMODULE(), 
VALUES(), 
)


#undef module

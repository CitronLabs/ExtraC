#pragma once
#include "extern.h"

typedef struct {u8 alloc[18];} FancyFunction_Alloc;

Class(FancyFunction,
INIT(),
FIELD(bool returned),
	
	#define fancy(...) static FancyFunction_Alloc __FANCY__ = {0}; 				\
     			for(init(FancyFunction, (inst(FancyFunction))&__FANCY__, __VA_ARGS__); 	\
			    !((inst(FancyFunction))&(__FANCY__))->returned; 						\
			    FancyFunction.DESTROY(generic &__FANCY__))

	#define defer defer: for(int i = 0; (i = FancyFunction.Defer((inst(FancyFunction))&__FANCY__)); FancyFunction.DESTROY(generic &__FANCY__))
	
	#define subfn(name, first, ...) (*name)(first, ...) = null; first; __VA_ARGS__; 
	#define routn(name) 	        (*name)() = null;

	#define freturn FancyFunction.DESTROY(generic &__FANCY__); return
      	
      	errvt method(FancyFunction, SetVarDel, void* var, void* del_func);
	bool method(FancyFunction, Defer);
)

Class(Coroutine, 
INIT(),
FIELD(),

	#define go(name)
	#define yield()
	#define stop()

	void(*Yield)();
	void(*Exit)();
	inst(Coroutine)(*This)();
	void (*Join)(inst(Coroutine) cr);
	void method(Coroutine, Start, void* args);
)


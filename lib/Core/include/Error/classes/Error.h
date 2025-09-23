#pragma once

#define __XC_ERROR__
#include "../pkg.h"
#include "../types.h"

Decl(Logger)

Class(Error,
__INIT(errvt errorcode; cstr message;),
__FIELD(errvt errorcode; cstr message;),

	inst(Logger) STDLOG;

	void method(Error, Print);
	errvt method(Error, Set,, const cstr errname, const char funcname[]);
      	noFail vmethod(Show);
      	noFail vmethod(Clear);
      	noFail vmethod(Hide);
      	errvt vmethod(Try, errvt* errors_to_catch, size_t num);
      	noFail vmethod(Throw);
	errvt vmethod(setLogger, inst(Logger) logger);
	errvt vmethod(setSignalHandler, u8 signals_to_handle, void fn(sighandler, ErrorSignal signal))
)

static inst(Error) core_geterr(){
	static data(Error) err;
	return &err;
}
static inst(Error)(*geterr)() = core_geterr;

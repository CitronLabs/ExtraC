#pragma once

#define __XC_ERROR__
#include "../pkg.h"
#include "../types.h"

Decl(Logger)

Class(Error,
INIT(errvt errorcode; cstr message;),
FIELD(errvt errorcode; cstr message;),

	inst(Logger) STDLOG;

	void method(Error, Print);
	errvt method(Error, Set,, const cstr errname, const char funcname[]);
      	noFail vmethod(Show);
      	noFail vmethod(Clear);
      	noFail vmethod(Hide);
      	errvt vmethod(Try, errvt* errors_to_catch, size_t num);
      	noFail vmethod(Throw);
	errvt vmethod(setLogger, inst(Logger) logger);
	errvt vmethod(setSignalHandler, u8 signals_to_handle, void (*sighandler)(ErrorSignal signal))
)



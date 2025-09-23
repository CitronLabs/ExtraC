#pragma once

#define __XC_ERROR__
#include "../pkg.h"
#include "../types.h"

Class(Logger,
__INIT( const cstr name;
      data(LogBook)
      * errorlog,
      * infolog
),
__FIELD(bool hideName),
      	#define loginfo(...) Logger.logf(Logger.std_logger, LOGGER_INFO, __VA_ARGS__,"\n", endprint)
      	#define logerr(...)  Logger.logf(Logger.std_logger, LOGGER_ERROR, __VA_ARGS__,"\n", endprint)
      	inst(Logger) std_logger;

	errvt method(Logger, logf,, LogType type, ...);
	errvt method(Logger, log,,  LogType type, inst(String) txt);
)

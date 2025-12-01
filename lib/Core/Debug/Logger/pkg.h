#pragma once
#define __XC_DEBUG__
#include "../pkg.h"
#include "../types.h"

#define package std

#define loginfo(...) Logger.logf(Logger.std_logger, LOGGER_INFO, __VA_ARGS__,"\n", endprint)
#define logerr(...)  Logger.logf(Logger.std_logger, LOGGER_ERROR, __VA_ARGS__,"\n", endprint)


Decl(String)

typefrom(i32, logID);

#define LOGGER_null -1

#define LOGGER_ERROR 1
#define LOGGER_INFO  0




Class(Logger, 
INIT(strc8 name; std_Stream *errorLog; std_Stream* infoLog), 
FMT(),
	u32 fn(nameLookup)(strc8 name);

private(
	std_String* 	name;
	std_ArrayList   logs;
)
){
      	std_Logger*  fn(getStdLogger)();
      	errvt 	     fn(setStdLogger)(std_Logger* logger);

	errvt method(Logger, log,   std_logID log, strc8 message);
	errvt method(Logger, logTo, strc8 name, strc8 message);

	std_logID method(Logger, newLog, std_Stream* stream);

	std_logID method(Logger, findLog,    strc8 name);
};

#undef package

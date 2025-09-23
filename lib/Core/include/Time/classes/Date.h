#pragma once

#define __XC_TIME__
#include "../pkg.h"
#include "../types.h"

#define date(string) (Date.fromString(push(Date), s(string)))
#define Date(string) (Date.fromString(new(Date),  s(string)))

Class(Date, 
__INIT(inst(Time) time),
__FIELD(
      TimeScale scale;
      union{
      	DateTime   time;
	JuilenTime juilen;
      }date;
),
	interface(Formatter);

      	inst(Time) method(Date, toTime);
      	inst(Date) method(Date, fromString,, inst(String) string);
)

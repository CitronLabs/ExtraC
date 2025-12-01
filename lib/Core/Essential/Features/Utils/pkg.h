#pragma once
#define __XC_FEATURE__
#include "../pkg.h"

#include "../Classes/pkg.h"

//=======EXPORTS========
#ifndef __XC_TYPES__
#define package std

Interface(Utils,
  namespace(mem,
  )
  namespace(charIs,
  )
  namespace(str,
	len_t fn(len)(strc8 str, len_t max_len);
  )
)

#define calloc(num, type) std.Utils.mem.alloc(num * type)




#undef package
#endif

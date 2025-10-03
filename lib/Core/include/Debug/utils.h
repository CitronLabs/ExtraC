#pragma once
#include "pkg.h"

#if __Debug
// utility for quickly removing debug statements
      #define debug(...) __VA_ARGS__
#else
      #define debug(...) 
#endif





#include "../../../pkg.h"
#include "../utils.h"

import(std)


SIZE(c8){  return sizeof(c8); }
SIZE(c16){ return sizeof(c16);}
SIZE(c32){ return sizeof(c32);}


COPY(c8){  *(c8*)where = *self; return where;}
COPY(c16){ *(c16*)where = *self; return where;}
COPY(c32){ *(c32*)where = *self; return where;}


construct(c8,FMT(),DEF()){passover}
construct(c16,FMT(),DEF()){passover}
construct(c32,FMT(),DEF()){passover}

construct(strc8,FMT(),DEF()){passover}
construct(strc16,FMT(),DEF()){passover}
construct(strc32,FMT(),DEF()){passover}

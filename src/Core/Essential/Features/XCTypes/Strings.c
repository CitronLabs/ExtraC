#include <XC.Core/pkg.c>

#define module c8

SIZE(c8){  return sizeof(c8); }
COPY(c8){  *(c8*)where = *self; return where;}
construct(c8,FMT(),DEF()){passover}

#undef module 
#define module c16

SIZE(c16){ return sizeof(c16);}
COPY(c16){ *(c16*)where = *self; return where;}
construct(c16,FMT(),DEF()){passover}

#undef module 
#define module c32

SIZE(c32){ return sizeof(c32);}
COPY(c32){ *(c32*)where = *self; return where;}
construct(c32,FMT(),DEF()){passover}


#undef module 
#define module strc8

construct(strc8,FMT(),DEF()){passover}

#undef module 
#define module strc16

construct(strc16,FMT(),DEF()){passover}

#undef module 
#define module strc32

construct(strc32,FMT(),DEF()){passover}

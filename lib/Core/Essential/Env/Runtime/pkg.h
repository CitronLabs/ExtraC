#pragma once
#define __XC_ENV__
#include "../pkg.h"
#include "../Lib/pkg.h"

#ifndef __XC_ENV_RUNTIME__

typedef struct {
  u8
    * __dataSection__start_ram,
    * __dataSection__end_ram,
    * __dataSection__start_rom,
	 
    * __bssSection__start_ram,
    * __bssSection__end_ram 
  ;

  void 
     (*__initArray__start)(),
     (*__initArray__end)(),
    *(*__heap__start)()
  ;
  len_t fn(pageSize);

} runtimeSetupData_Core;


Module(runtime){
  namespace(setup,
    runtimeSetupData_Core* fn(core)();
  )
  namespace(info,
    const bool implemented;
    len_t fn(pagesAlloced)();
  )
};


#endif


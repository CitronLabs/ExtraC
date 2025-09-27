#pragma once
#include "../libc.h"
#include "classes.h"
#include "macros/esc_seq.h"

#define geterr() core_geterr();
#define quiet() __err_hide(); for(int i = 1; i--; __err_show())

#define ERR(code, msg) Error_Set(&(err_t){code, #code}, msg, __func__)
#define NOT_IMPLEM(returnval) ERR(ERR_NOTIMPLEM, "not implemented yet..."); return returnval;
#define errnm  (geterr()->errorcode)
#define errstr (geterr()->message)
#define OK ERR_NONE

#define iferr(errorable) for(errvt err = (errorable); err; Error_Clear())

typedef enum{ 
	#define __ERROR_CODES__
	#include "../config.h"
	#undef __ERROR_CODES__
}XC_ERROR_CODES;


typedef struct {errvt code; const char* msg} err_t;

static inline err_t* core_geterr();
static inline void __err_hide();
static inline void __err_show();


errvt Error_Set(err_t* err, const char* errmsg, const char funcname[]);
void Error_Clear();




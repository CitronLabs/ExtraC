#pragma once
#define __XC_ENV_LIB_C__
#include "../pkg.h"



#ifndef __XC_LIBC_HIDETYPES__
	// Type for wide character return values/constants (often int or unsigned int)
	typedef unsigned int wint_t; 

	// Type for wide-character classification
	typedef int wctype_t; 

	// Type for wide-character transformation descriptor
	typedef int wctrans_t;
#endif

Interface(C_WSTR_TYPE,
	int 	  fn(iswalnum)(wint_t wc);
	int 	  fn(iswalpha)(wint_t wc);
	int 	  fn(iswblank)(wint_t wc);
	int 	  fn(iswcntrl)(wint_t wc);
	int 	  fn(iswdigit)(wint_t wc);
	int 	  fn(iswgraph)(wint_t wc);
	int 	  fn(iswlower)(wint_t wc);
	int 	  fn(iswprint)(wint_t wc);
	int 	  fn(iswpunct)(wint_t wc);
	int 	  fn(iswspace)(wint_t wc);
	int 	  fn(iswupper)(wint_t wc);
	int 	  fn(iswxdigit)(wint_t wc);
	
	wint_t 	  fn(towlower)(wint_t wc);
	wint_t 	  fn(towupper)(wint_t wc);
	
	wctype_t  fn(wctype)(const char *property);
	int 	  fn(iswctype)(wint_t wc, wctype_t desc);
	
	wint_t 	  fn(towctrans)(wint_t wc, wctrans_t desc);
	wctrans_t fn(wctrans)(const char *property);
)


#ifndef __XC_LIBC_HIDEMACROS__

// A constant that cannot correspond to a valid wide character
#define WEOF ((wint_t)-1)

#define iswalnum(wc)		C.str.wide.type.iswalnum(wc)	   
#define iswalpha(wc)            C.str.wide.type.iswalpha(wc)       
#define iswblank(wc)            C.str.wide.type.iswblank(wc)       
#define iswcntrl(wc)            C.str.wide.type.iswcntrl(wc)       
#define iswdigit(wc)            C.str.wide.type.iswdigit(wc)       
#define iswgraph(wc)            C.str.wide.type.iswgraph(wc)       
#define iswlower(wc)            C.str.wide.type.iswlower(wc)       
#define iswprint(wc)            C.str.wide.type.iswprint(wc)       
#define iswpunct(wc)            C.str.wide.type.iswpunct(wc)       
#define iswspace(wc)            C.str.wide.type.iswspace(wc)       
#define iswupper(wc)            C.str.wide.type.iswupper(wc)       
#define iswxdigit(wc)           C.str.wide.type.iswxdigit(wc)      
                                                                   
#define towlower(wc)            C.str.wide.type.towlower(wc)       
#define towupper(wc)            C.str.wide.type.towupper(wc)       
                                                                   
#define wctype(property)	C.str.wide.type.wctype(property)   
#define iswctype(wc, desc)      C.str.wide.type.iswctype(wc, desc) 
                                                                   
#define towctrans(wc, desc)     C.str.wide.type.towctrans(wc, desc)
#define wctrans(property)       C.str.wide.type.wctrans(property)  
                                
#endif                               
                                
                                
                                
                                
                                
                                
                                
                                
                                

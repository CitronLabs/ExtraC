#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <errno.h>
#include <setjmp.h>
#include <uchar.h>
#include <wctype.h>
#include <wchar.h>
#include <float.h>
#include <signal.h>
#include <ctype.h>


#define __XC_LIBC_HIDETYPES__
#define __XC_LIBC_HIDEMACROS__
#include "pkg.h"

Impl(C){
  .io = {
    .fmt = {
       	.fprintf  	= fprintf,
	.fscanf    	= fscanf,
	.printf    	= printf,
	.sprintf   	= sprintf,
	.snprintf  	= snprintf,
	.vfprintf  	= vfprintf,
	.vprintf   	= vprintf,
	.vsprintf  	= vsprintf,
	.vsnprintf 	= vsnprintf,
	.scanf     	= scanf,
	.sscanf    	= sscanf,
	.fwprintf	= fwprintf,
	.fwscanf	= fwscanf,	     
	.swprintf	= swprintf,
	.swscanf	= swscanf,	     
	.vfwprintf	= vfwprintf,
	.vfwscanf	= vfwscanf,
	.vswprintf	= vswprintf,
	.vswscanf	= vswscanf,
	.vwprintf	= vwprintf,
	.vwscanf	= vwscanf,	     
	.wprintf	= wprintf,	     
	.wscanf		= wscanf,
    },
    .str = {
   	.fgetc  	= fgetc,
	.fgets          = fgets,
	.fputc          = fputc,
	.fputs          = fputs,
	.getc           = getc ,
	.getchar        = getchar,
	.putc           = putc,
	.putchar        = putchar,
	.puts           = puts,
	.ungetc         = ungetc ,
	.fgetwc		= fgetwc,
	.fgetws		= fgetws,
	.fputwc		= fputwc,
	.fputws		= fputws,
	.getwc 		= getwc,
	.getwchar	= getwchar,
	.putwc		= putwc,
	.putwchar	= putwchar,
	.ungetwc	= ungetwc,		
    },
    .strm = {
 	.fclose		= fclose,
    	.feof		= feof,		
    	.fflush		= fflush,		
    	.fopen		= fopen,		
    	.fread		= fread,		
    	.freopen	= freopen,		
       	.fseek		= fseek,		
       	.fsetpos	= fsetpos,		
       	.ftell		= ftell,		
       	.fwrite		= fwrite,		
    	.fgetpos	= fgetpos,		
   	.rewind		= rewind,		
   	.setbuf		= setbuf,		
   	.setvbuf	= setvbuf,		
    },
    .extrn = {
   	.tmpfile	= tmpfile,
   	.tmpnam	        = tmpnam,	        
   	.remove	        = remove,	        
   	.rename	        = rename,	        
    },
    	.ferror    	= ferror,
   	.clearerr  	= clearerr,
      	.perror    	= perror,  
  },
  .str = {
    .base = {
      .mem = {
	.memchr  	= memchr,
	.memcmp  	= memcmp,
	.memcpy  	= memcpy,
	.memmove 	= memmove,
	.memset  	= memset,
      },
      .ops = {
	.strcat		= strcat,	
	.strncat	= strncat,	
	.strchr		= strchr,	
	.strcmp		= strcmp,	
	.strncmp	= strncmp,	
	.strcoll	= strcoll,	
	.strcpy		= strcpy,	
	.strncpy	= strncpy,	
	.strcspn	= strcspn,	
	.strerror	= strerror,
	.strlen		= strlen,	
	.strnlen	= strnlen,	
	.strpbrk	= strpbrk,	
	.strrchr	= strrchr,	
	.strspn		= strspn,	
	.strstr		= strstr,	
	.strtok		= strtok,	
	.strxfrm	= strxfrm	
      },
      .type = {
	.isalnum	= isalnum,		
	.isalpha	= isalpha,		
	.isblank	= isblank,		
	.iscntrl	= iscntrl,		
	.isdigit	= isdigit,		
	.isgraph	= isgraph,		
	.islower	= islower,		
	.isprint	= isprint,		
	.ispunct	= ispunct,		
	.isspace	= isspace,		
	.isupper	= isupper,		
	.isxdigit	= isxdigit,	
	.tolower	= tolower,		
	.toupper	= toupper,		
      },
      .utils = {
	.atoi 		= atoi,
	.atol    	= atol,
	.strtod  	= strtod,
	.strtol  	= strtol,
	.strtoul 	= strtoul,
      }
    },
    .wide = {
      .mem = {
	.wmemchr  	= wmemchr,
	.wmemcmp  	= wmemcmp,
	.wmemcpy  	= wmemcpy,
	.wmemmove 	= wmemmove,
	.wmemset  	= wmemset,
      },
      .ops = {
	.wcscat		= wcscat,	
	.wcsncat	= wcsncat,	
	.wcschr		= wcschr,	
	.wcscmp		= wcscmp,	
	.wcsncmp	= wcsncmp,	
	.wcscoll	= wcscoll,	
	.wcscpy		= wcscpy,	
	.wcsncpy	= wcsncpy,	
	.wcscspn	= wcscspn,	
	.wcslen		= wcslen,	
	.wcspbrk	= wcspbrk,	
	.wcsrchr	= wcsrchr,	
	.wcsspn		= wcsspn,	
	.wcsstr		= wcsstr,	
	.wcstok		= wcstok,	
	.wcsxfrm	= wcsxfrm	
      },
      .type = {
	.iswalnum	= iswalnum,		
	.iswalpha	= iswalpha,		
	.iswblank	= iswblank,		
	.iswcntrl	= iswcntrl,		
	.iswdigit	= iswdigit,		
	.iswgraph	= iswgraph,		
	.iswlower	= iswlower,		
	.iswprint	= iswprint,		
	.iswpunct	= iswpunct,		
	.iswspace	= iswspace,		
	.iswupper	= iswupper,		
	.iswxdigit	= iswxdigit,	
	.towlower	= towlower,		
	.towupper	= towupper,		
	.wctype		= wctype, 
	.iswctype	= iswctype,
	.towctrans	= towctrans,
	.wctrans	= wctrans,	
      },
      .utils = {
	.mblen		= mblen,
	.mbstowcs	= mbstowcs,
	.mbtowc		= mbtowc,		
	.wcstombs	= wcstombs,	
	.wctomb		= wctomb,		
	.btowc		= btowc,	
	.mbrlen		= mbrlen,		
	.mbrtowc	= mbrtowc,	      
	.mbsinit	= mbsinit,	      
	.mbsrtowcs	= mbsrtowcs,	
	.wcrtomb	= wcrtomb,	      
	.wcsrtombs	= wcsrtombs,	
	.fwide		= fwide,		
	.wcstod		= wcstod,		
	.wcstof		= wcstof,		
	.wcstol		= wcstol,		
	.wcstold	= wcstold,	      
	.wctob		= wctob,		
	.wcstoll	= wcstoll,	      
	.wcstoul	= wcstoul,	      
	.wcstoull	= wcstoull,	
      }
    }
  },
  .lib = {
    .math = {
	.abs		= abs,		
	.div		= div,	
	.labs		= labs,	
	.ldiv		= ldiv,	
	.rand		= rand,	
	.srand		= srand,	
    },
    .utils = {
	.abort		= abort,	
	.atexit		= atexit,	
	.exit		= exit,	
	.getenv		= getenv,	
	.system		= system,	
	.bsearch	= bsearch,
	.qsort		= qsort,	
    },
    .alloc = {
	.calloc		= calloc,	
	.free		= free,	
	.malloc		= malloc,	
	.alloca		= alloca,	
	.realloc	= realloc,
    }
  },
  .sig = {
	.raise  	= raise,
	.signal 	= signal,
  },
  .jmp = {
	.longjmp 	= longjmp,
	.setjmp 	= setjmp,
  },
  .locale = {
	.localeconv 	= localeconv,
	.setlocale  	= setlocale
  },
  .time = {
	.asctime	= asctime,
	.clock		= clock,	 
	.ctime		= ctime,	 
	.difftime	= difftime,
	.gmtime		= gmtime,	 
	.localtime	= localtime,
	.mktime		= mktime,	 
	.strftime	= strftime,
	.time		= time,	 
	.wcsftime	= wcsftime
  },
};

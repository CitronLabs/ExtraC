#pragma once
#define __XC_ENV_LIB_C__

#include "../pkg.h"
#include "stdarg.h"
#include "stdlib.h"

#ifndef __XC_LIBC_HIDETYPES__

typedef struct FILE FILE;
typedef size_t fpos_t;

#endif

extern FILE
	*stderr, *stdout, *stdin; 

Interface(C_IO,
    namespace(fmt,
       	int    fn(fprintf)   (FILE *stream, const char *format, ...);
	int    fn(fscanf)    (FILE *stream, const char *format, ...);
       	int    fn(printf)    (const char *format, ...);
       	int    fn(sprintf)   (char *str, const char *format, ...);
       	int    fn(snprintf)  (char *str, size_t n, const char *format, ...);
  	int    fn(vfprintf)  (FILE *stream, const char *format, va_list arg);
  	int    fn(vprintf)   (const char *format, va_list arg);
  	int    fn(vsprintf)  (char *str, const char *format, va_list arg);
  	int    fn(vsnprintf) (char *str, size_t n, const char *format, va_list arg);
  	int    fn(scanf)     (const char *format, ...);
	int    fn(sscanf)    (const char *str, const char *format, ...);
	
	int      fn(fwprintf)(FILE *stream, const wchar_t *format, ...);
	int      fn(fwscanf)(FILE *stream, const wchar_t *format, ...);
	int 	 fn(swprintf)(wchar_t *s, size_t n, const wchar_t *format, ...);
	int 	 fn(swscanf)(const wchar_t *s, const wchar_t *format, ...);
	int 	 fn(vfwprintf)(FILE *stream, const wchar_t *format, va_list arg);
	int 	 fn(vfwscanf)(FILE *stream, const wchar_t *format, va_list arg);
	int 	 fn(vswprintf)(wchar_t *s, size_t n, const wchar_t *format, va_list arg);
	int 	 fn(vswscanf)(const wchar_t *s, const wchar_t *format, va_list arg);
	int 	 fn(vwprintf)(const wchar_t *format, va_list arg);
	int 	 fn(vwscanf)(const wchar_t *format, va_list arg);
	int 	 fn(wprintf)(const wchar_t *format, ...);
	int 	 fn(wscanf)(const wchar_t *format, ...);
    )
    namespace(strm,
 	int    fn(fclose)    (FILE *stream);
    	int    fn(feof)      (FILE *stream);
    	int    fn(fflush)    (FILE *stream);
    	FILE*  fn(fopen)     (const char *filename, const char *mode);
    	size_t fn(fread)     (void *ptr, size_t size, size_t nmemb, FILE *stream);
    	FILE*  fn(freopen)   (const char *filename, const char *mode, FILE *stream);
       	int    fn(fseek)     (FILE *stream, long int offset, int whence);
       	int    fn(fsetpos)   (FILE *stream, const fpos_t *pos);
       	long   fn(ftell)     (FILE *stream);
       	size_t fn(fwrite)    (const void *ptr, size_t size, size_t nmemb, FILE *stream);
    	int    fn(fgetpos)   (FILE *stream, fpos_t *pos);
   	void   fn(rewind)    (FILE *stream);
   	void   fn(setbuf)    (FILE *stream, char *buffer);
   	int    fn(setvbuf)   (FILE *stream, char *buffer, int mode, size_t size);
    )
    namespace(extrn,
   	FILE*  fn(tmpfile)   (void);
   	char*  fn(tmpnam)    (char *str);
   	int    fn(remove)    (const char *filename);
   	int    fn(rename)    (const char *old_filename, const char *new_filename);
    )
    	int    fn(ferror)    (FILE *stream);
   	void   fn(clearerr)  (FILE *stream);
      	void   fn(perror)    (const char *str);
    namespace(str,
   	int    fn(fgetc)     (FILE *stream);
   	char*  fn(fgets)     (char *str, int n, FILE *stream);
   	int    fn(fputc)     (int c, FILE *stream);
   	int    fn(fputs)     (const char *str, FILE *stream);

      	int    fn(getc)      (FILE *stream);
      	int    fn(getchar)   (void);
      	int    fn(putc)      (int c, FILE *stream);
      	int    fn(putchar)   (int c);
      	int    fn(puts)      (const char *str);
      	int    fn(ungetc)    (int c, FILE *stream);
	
	wint_t 	 fn(fgetwc)(FILE *stream);
	wchar_t* fn(fgetws)(wchar_t *s, int n, FILE *stream);
	wint_t   fn(fputwc)(wchar_t wc, FILE *stream);
	int      fn(fputws)(const wchar_t *s, FILE *stream);
	
	wint_t   fn(getwc)	(FILE *stream);
	wint_t   fn(getwchar)(void);
	wint_t   fn(putwc)(wchar_t wc, FILE *stream);
	wint_t   fn(putwchar)(wchar_t wc);
	wint_t 	 fn(ungetwc)(wint_t wc, FILE *stream);
    )
);

#ifndef __XC_LIBC_HIDEMACROS__

#define fprintf(stream, format, ...) 	C.io.fmt.fprintf 	(stream, format, __VA_ARGS__)
#define fscanf(stream, format, ...)	C.io.fmt.fscanf  	(stream, format, __VA_ARGS__)
#define printf(format, ...)		C.io.fmt.printf  	(format, __VA_ARGS__)
#define sprintf(str, format, ...)	C.io.fmt.sprintf  	(str,    format, __VA_ARGS__)
#define snprintf(str, n, format, ...)	C.io.fmt.snprintf  	(str, n, format, __VA_ARGS__)
#define vfprintf(stream, format, arg)	C.io.fmt.vfprintf 	(stream, format, arg)
#define vprintf(format, arg)		C.io.fmt.vprintf	(format, arg)
#define vsprintf(str, format, arg)	C.io.fmt.vsprintf	(str, format, arg)
#define vsnprintf(str, n, format, arg)	C.io.fmt.vsnprintf	(str, n, format, arg)
#define scanf(format, ...)		C.io.fmt.scanf		(format, __VA_ARGS__)
#define sscanf(str, format, ...)	C.io.fmt.sscanf		(str, format, __VA_ARGS__)


#define fwprintf(stream, format, ...)	C.io.fmt.fwprintf(stream, format, __VA_ARGS__)
#define fwscanf(stream, format, ...)	C.io.fmt.fwscanf(stream, format, __VA_ARGS__)
#define swprintf(s, n, format, ...)	C.io.fmt.swprintf(s, n, format, __VA_ARGS__)
#define swscanf(s, format, ...)		C.io.fmt.swscanf(s, format, __VA_ARGS__)	
#define vfwprintf(stream, format, arg)	C.io.fmt.vfwprintf(stream, format, arg)	
#define vfwscanf(stream, format, arg)	C.io.fmt.vfwscanf(stream, format, arg)	
#define vswprintf(s, n, format, arg)	C.io.fmt.vswprintf(s, n, format, arg)	
#define vswscanf(s, format, arg)	C.io.fmt.vswscanf(s, format, arg)	
#define vwprintf(format, arg)		C.io.fmt.vwprintf(format, arg)		
#define vwscanf(format, arg)		C.io.fmt.vwscanf(format, arg)		
#define wprintf(format, ...)		C.io.fmt.wprintf(format, __VA_ARGS__)		
#define wscanf(format, ...)		C.io.fmt.wscanf(format, __VA_ARGS__)		



#define fclose(stream)				C.io.strm.fclose	(stream)
#define feof(stream)				C.io.strm.feof		(stream)
#define fflush(stream)				C.io.strm.fflush 	(stream)
#define fopen(filename, mode)			C.io.strm.fopen		(filename, mode)
#define fread(ptr, size, nmemb, stream) 	C.io.strm.fread		(ptr, size, nmemb, stream)
#define freopen(filename, mode, stream) 	C.io.strm.freopen 	(filename, mode, stream)
#define fseek(stream, offset, whence)   	C.io.strm.fseek		(stream, offset, whence)
#define fsetpos(stream, pos)            	C.io.strm.fsetpos	(stream, pos)
#define ftell(stream)                   	C.io.strm.ftell 	(stream)
#define fwrite(ptr, size, nmemb, stream)	C.io.strm.fwrite	(ptr, size, nmemb, stream)
#define fgetpos(stream, pos)            	C.io.strm.fgetpos	(stream, pos)
#define rewind(stream)                  	C.io.strm.rewind 	(stream)
#define setbuf(stream, buffer)			C.io.strm.setbuf 	(stream, buffer)
#define setvbuf(stream, buffer, mode, size)     C.io.strm.setvbuf	(stream, buffer, mode, size)

#define tmpfile()				C.io.extrn.tmpfile()
#define tmpnam(str)				C.io.extrn.tmpnam(str)
#define remove(filename)			C.io.extrn.remove(filename)
#define rename(old_filename, new_filename)	C.io.extrn.rename(old_filename, new_filename)

#define ferror(stream)		C.io.extrn.ferror	(stream)
#define clearerr(stream)        C.io.extrn.clearerr	(stream)
#define perror(str)             C.io.extrn.perror	(str)
                                
#define fgetc(stream)		C.io.str.fgetc		(stream)
#define fgets(str, n, stream)   C.io.str.fgets		(str, n, stream)
#define fputc(c, stream)        C.io.str.fputc		(c, stream)
#define fputs(str, stream)      C.io.str.fputs		(str, stream)

#define getc(stream)            C.io.str.getc		(stream)
#define getchar()              	C.io.str.getchar	()
#define gets(str)               C.io.str.gets		(str)
#define putc(c, stream)         C.io.str.putc		(c, stream)
#define putchar(c)              C.io.str.putchar	(c)
#define puts(str)               C.io.str.puts		(str)
#define ungetc(c, stream)       C.io.str.ungetc		(c, stream)

#endif


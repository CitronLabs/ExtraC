#pragma once
#define __XC_OS_MEDIA__
#include "../pkg.h"
#include "../types.h"



#ifndef __XC_OS_MEDIA_Stream__
#define package os_Media

Class(Stream,
INIT(os_Media_Type type),
FMT(),

      os_Media_Type type;
      std_Stream stream;

private(
	union {
		os_Media_Audio_Desc 	   audio;
      		os_Media_Vertex_Desc       vertex;
      		os_Media_Layout_Desc       layout;
      		os_Media_Color_Desc        color;
      		std_Object*		   other;
	} spec;
)){



};

#undef package
#endif

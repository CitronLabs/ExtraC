#pragma once
#define __XC_OS_MEDIA__
#include "pkg.h"

#define package os_Media

typefrom(u32, FourCC);

typefrom(word, Type);

type(Color_Desc,
    u16 
     	red_len,   red_off,
     	blue_len,  blue_off,
     	green_len, green_off,
     	alpha_len, alpha_off
    ;

    bool floating;

    u16  dimension_num;
    u32  dimension_size[];
)


type(Vertex_Desc,
   	 u16  attribute_num;
   	 u32  attribute_size[];
)

type(Layout_Desc,
	len_t elementIDMax;

    	u16  dimension_num;
    	u32  dimension_size[];
)

type(Audio_Desc,
	u32 sample_rate;    
	u16 channels;       
	u16 bits_per_sample; 
)

#undef package

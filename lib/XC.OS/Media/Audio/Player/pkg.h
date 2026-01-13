#pragma once
#define __XC_OS_MEDIA_AUDIO__
#include "../pkg.h"

#define package os_Audio

Class(Player,
INIT(),
FMT(),
    	u32 sample_rate;     
    	u16 channels;        
    	u16 bits_per_sample; 
    	struct { float x, y, z; } panning;
    	float gain;
){
      	errvt 		imethod(play);
      	errvt 		imethod(stop);
      	errvt 		imethod(update);
	bool 		imethod(isPlaying);
	os_Handle  	imethod(getHandle);

	errvt  		imethod(getProcessor);
	errvt  		imethod(setProcessor);
	errvt		imethod(enumProcessors);

      	os_Media_Stream* imethod(stream, os_Media_Stream* stream);

};


#undef package

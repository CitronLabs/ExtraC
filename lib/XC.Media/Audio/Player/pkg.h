#pragma once
#define __XC_MEDIA_AUDIO__
#include "../pkg.h"


#ifndef __XC_MEDIA_AUDIO_PLAYER__
#include "types.h"

#define package std_Audio

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

	errvt  		imethod(getProcessor);
	errvt  		imethod(setProcessor);
	errvt		imethod(enumProcessors);

      	std_Media_Stream* imethod(stream, std_Media_Stream* stream);

};

#undef package
#endif


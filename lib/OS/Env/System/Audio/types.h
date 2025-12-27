#pragma once
#define __XC_OS_ENV_SYSTEM_AUDIO__
#include "pkg.h"

#define package os_Env_Audio

#define AudioHandle 	os_Env_Audio_Handle
#define AudioDirection 	os_Env_Audio_Direction
#define AudioSpec 	os_Env_Audio_Spec
#define AudioDevice 	os_Env_Audio_Device

typefrom(pntr, Handle) // aka an audio stream

typefrom(u8, Direction);

type(Spec,
    u32 sample_rate;    // e.g., 44100, 48000 Hz
    u16 channels;       // e.g., 1 (mono), 2 (stereo)
    u16 bits_per_sample; // e.g., 16, 24, 32 bits
);

type(Device,
	std_String 
		name, 
	 	model,
		manufacturer;
	
	void* uniqueID;
    
	AudioDirection direction;

	u32  maxChannels;              	// Maximum number of channels supported.
    	u32* supportedSampleRates; 	// A list of supported sample rates (terminated by 0).
    	u32* supportedBitsPerSample;  	// A list of supported bits per sample (terminated by 0).
    	u32  minLatencyFrames;         	// Minimum latency in frames.
    	u32  maxLatencyFrames;         	// Maximum latency in frames.
    	u32  defaultLatencyFrames;     	// Default latency in frames.

)


typefrom(word, EventType)


type(Event,
	AudioHandle handle;
	os_Env_Audio_EventType type;
)

#undef package

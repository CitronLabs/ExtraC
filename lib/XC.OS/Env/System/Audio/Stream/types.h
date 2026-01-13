#pragma once
#define __XC_OS_ENV_SYSTEM_AUDIO_STREAM__
#include "pkg.h"

#define package os_Env_Audio_Stream

type(Frame,
	u32    	frameCount;    	
	u32 	busID;

	u64    sampleTime;
	double streamTime;
    
	union {
	void** pcm;      // [channel][sample] - Multi-channel audio
        void* raw;       // Generic access
	
	struct {
		u32 delta_frames;   // Offset from the start of the current frame (0 to frame_count - 1)
		u8  status;         // MIDI Status (e.g., 0x90 for Note On)
		u8  data1;          // Note number / CC number
		u8  data2;          // Velocity / Value
		u8  channel;        // MIDI Channel (1-16)
	}midi;
        
        float* envelope;  
          struct {
            float* magnitudes;
            float* phases;
            u32    bin_count;
          } fft;             
	} data;

	struct {
		u32 silent 	  	: 1;
		u32 discontinuity 	: 1;
     		u32 resolution	  	: 2;
     		u32 data_type	   	: 3;
		u32 overdue		: 1;
		u32 dropped		: 1;
		u32 run_position	: 2;
	
		u32 clock_uncertain	: 1;

		u32 planar		: 1;
		u32 read_write		: 2;
     	} flags;          
)
type(Spec,
    u32 sample_rate;     // e.g., 44100, 48000 Hz
    u16 channels;        // e.g., 1 (mono), 2 (stereo)
    u16 bits_per_sample; // e.g., 16, 24, 32 bits
);

type(Device,
	bool 			primary;
)
#undef package

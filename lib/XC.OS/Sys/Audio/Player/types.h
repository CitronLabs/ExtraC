#pragma once
#define __XC_OS_SYSTEM_AUDIO_PLAYER__
#include "pkg.h"

#define package os_Sys_Audio_Player
type(Device,
    	u32  minLatencyFrames;         	// Minimum latency in frames.
    	u32  maxLatencyFrames;         	// Maximum latency in frames.
    	u32  defaultLatencyFrames;     	// Default latency in frames.

	bool primary;
)


type(Settings,
    u32 settings_set_flags;

    u32 sample_rate;     // e.g., 44100, 48000 Hz
    u16 channels;        // e.g., 1 (mono), 2 (stereo)
    u16 bits_per_sample; // e.g., 16, 24, 32 bits
    struct { float x, y, z; } panning;
    float gain;
);

#undef package

#pragma once
#define __XC_OS_SYSTEM_AUDIO_PROCESSOR__
#include "pkg.h"

#define package os_Sys_Audio_Processor

typefrom(u32, ResourceID);

type(Device,
    u32 execModel;
    u32 features;
    
 
    struct {
        u32 max_sample_rate;    // e.g., 192000
        u32 max_channels;       // e.g., 8 (7.1) or 64
        bool hardware_mixing;
        bool supports_float32;  // True if backend prefers f32 over i16
    } limits;

    len_t  frame_alignment;       // Bytes per multi-channel frame
    u32    preferred_buffer_size; // Optimal buffer size for low latency

)

#undef package

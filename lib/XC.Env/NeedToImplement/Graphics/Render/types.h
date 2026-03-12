#pragma once
#define __XC_OS_SYSTEM_GRAPHICS_RENDER__
#include "pkg.h"

#define package os_Sys_Graphics_Render

typefrom(u32, ResourceID);

type(Device,
    u32 execModel;
    u32 features;
    u32 memoryType;
    
    struct {
        u32 maxTextureDim;
        u32 maxVertexCount;
        u32 maxRenderTargets;
        bool hardwareAccelerated;
        bool unifiedMemory;
        bool supportsClipping;
    } limits;

    struct {
        bool multiViewport;
        bool occlusionQuery;
        bool timestampQueries;
        u8   maxAnisotropy;
    } supports;

    struct {
        bool supportsTearing;
        u32  minImageCount;
    } swapchain;

    len_t minimumAlignment;
    len_t rowPitchAlignment;
    bool  threadSafeSubmission;
)



#undef package

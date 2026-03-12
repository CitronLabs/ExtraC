#pragma once
#define __XC_MEDIA_GRAPHICS_RENDER__
#include "pkg.h"

#define package std_Renderer




type(Info,

    struct {
        bool supportsTearing;
        u32  minImageCount;
    } swapchain;

    struct {
        len_t rowPitchAlignment;
        len_t minimumAlignment;
        u32   maxTextureDim;
        u32   maxVertexCount;
        u32   maxRenderTargets;
        u8    maxAnisotropy;
    } limits;

    struct {
	bool immediate		: 1;
	bool deferred		: 1;
	bool consumer		: 1;
	bool asyncCompute	: 1;
	bool remote		: 1;
	bool virtualized	: 1;
    } execModel;

    struct {
     	bool unified 		: 1;
	bool deviceLocal	: 1;
	bool hostVisible	: 1;
	bool hostCoherent 	: 1;
	bool cached		: 1;
	bool protected		: 1;
    } memoryType;

    struct {
    	bool threadSafe		: 1;
     	bool clipping   	: 1;
     	bool hardwareAccel	: 1;
        bool multiViewport	: 1;
        bool occlusionQuery	: 1;
        bool timestampQueries	: 1;
        bool pixelBlit		: 1;
        bool geometry		: 1;
        bool text		: 1;
        bool layout		: 1;
        bool compute		: 1;
        bool rayTracing		: 1;
        bool meshShading	: 1;
        bool videoDecode	: 1;
        bool HDR10		: 1;
        bool alphaBlend		: 1;
        bool scalingHardware	: 1;
    } supports;
)

#undef package

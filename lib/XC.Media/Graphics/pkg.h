#pragma once
#define __XC_MEDIA__
#include "../pkg.h"
#include "../Canvas/pkg.h"
#include "../Stream/pkg.h"

#ifndef __XC_MEDIA_GRAPHICS__

#include "Terminal/pkg.h"
#include "Screen/pkg.h"
#include "Window/pkg.h"
#include "Camera/pkg.h"
#include "Renderer/pkg.h"
#include "types.h"

#define package std

Interface(Graphics,
	interface(std_Terminal) Terminal;	
	interface(std_Screen)   Screen;	
	interface(std_Window)   Window;	
	interface(std_Camera)   Camera;	
	interface(std_Renderer) Renderer;	
)

#undef package
#undef GFXColor 
#endif

#pragma once
#include "Test.pkg.h"
#include "Network.pkg.h"
#include "Render.pkg.h"

Module(builtin){
	interface(renderkit)  	Render;
	interface(weaver)  	Network;
	interface(labrat)  	Test;
};

#undef package



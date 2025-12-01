#pragma once
#include "../../os.h"

Class(Speaker,
INIT(),
FIELD(),

)

Class(Microphone,
INIT(),
FIELD(),

)

static inst(Speaker) defaultSpeaker 	= null;
static inst(Speaker) defaultMicrophone  = null;

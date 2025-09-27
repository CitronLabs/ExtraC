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

static inst(Speaker) defaultSpeaker 	= NULL;
static inst(Speaker) defaultMicrophone  = NULL;

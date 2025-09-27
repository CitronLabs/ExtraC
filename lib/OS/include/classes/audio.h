#pragma once
#include "../OS.h"

Decl(AudioDevice);

Data(Audio,
INIT(arry(float) samples; size_t len),
	inhert(Buffer);
	AudioSpec spec;
);

Class(Jack,
INIT(inst(AudioDevice) device; bool direction),
FIELD(),
	errvt method(Jack, sendAudio,, inst(Audio) audio);
	errvt method(Jack, connect,,   inst(Audio) audio);
);

Class(AudioDevice,
INIT(errvt fn(callback, inst(Audio) audio, pntr params)),
FIELD(),
	errvt method(AudioDevice, setParamsStruct,, pntr params, size_t len);
);


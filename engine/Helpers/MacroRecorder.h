#pragma once

#include "../OWEngine/OWEngine.h"

/*
	Class to capture UserInput, persist them and play them back.
	Adds itself to UserInput as a listener to record Macroes.
	When play back is enabled then it is the source of UserInput for
	the movie.
	Has default keystrokes listed in UserInput
	Can be started from UserCommand line?
	Works in conjunction with class SaveAndRestore to provide the same state on playback
*/
class OWENGINE_API MacroRecorder
{
public:
	MacroRecorder() {}
};
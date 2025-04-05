#pragma once
#include <queue>

#include <Core/Movie.h>

#include "NMSUserInput.h"

class GLApplication;
class Logger;
class NMSMovie: public Movie
{
public:
	NMSMovie(Camera* _camera, Logger* logger);
	void init(GLApplication* app, UserInput* ui, MacroRecorder* recorder) override;
	void preRun();
protected:
private:
};
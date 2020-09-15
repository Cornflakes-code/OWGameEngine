#pragma once
#include <queue>

#include <Core/Movie.h>

#include "NMSUserInput.h"

class GLApplication;

class NMSMovie: public Movie
{
public:
	NMSMovie(Camera* _camera);
	void init(GLApplication* app, UserInput* ui, MacroRecorder* recorder) override;
	void preRun();
	void render(const ScenePhysicsState* state) override;
protected:
private:
};
#pragma once

#include "../OWEngine/OWEngine.h"

#include "Scene.h"
/*
	A core Scene required as a placeholder for the Game Loop implementation in Movie.cpp
*/

class QuitScene : public Scene
{
	// The framework requires a scene with a name of OWUtils::quitSceneName().
	// It will be the last scene loaded and can (not must!) be overridden to clear resource.
public:
	QuitScene(const Movie* movie);
	std::string name() const {
		return Scene::finalSceneName();
	}
	void activate(const std::string& previousScene, Camera* camera, unsigned int callCount) override;
	bool processUserCommands(const UserInput::AnyInput& userInput, std::string& nextScene, Camera* camera) override;
	void deActivate(const Camera* camera) override;
protected:
	void doSetupScene() override;
};

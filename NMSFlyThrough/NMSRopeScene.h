#pragma once

#include "NMSScene.h"

/*
	An implementation of a Scene for the NMS game.
	Will be moved out of the engine to a different repo
*/

class NMSRopeScene : public NMSScene
{
public:
	NMSRopeScene(const Movie* movie);
	std::string name() const { return "Rope"; }
	void activate(const std::string& previousScene,
		Camera* camera, unsigned int callCount) override;
	void deActivate(const Camera* camera) override;
	bool processUserCommands(const UserInput::AnyInput& userInput,
		std::string& nextScene,
		Camera* camera) override;
protected:
	void doSetupScene() override;
private:
	glm::vec3 mCameraFocus = glm::vec3(0);
};


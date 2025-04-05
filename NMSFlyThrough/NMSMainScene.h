#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "NMSScene.h"

/*
	An implementation of a Scene for the NMS game.
	Will be moved out of the engine to a different repo
*/

class NMSMainScene : public NMSScene
{
public:
	glm::vec3 mCameraPosition = glm::vec3(-1.48475, 1.77853, -0.553399);
	glm::vec3 mLookAt = glm::vec3(-0.799972, 1.20117, -0.10871);
	NMSMainScene(const Movie* movie);
	std::string name() const { return "Main"; }
	void activate(const std::string& previousScene, 
				  Camera* camera, unsigned int callCount) override;
	bool processUserCommands(const UserInput::AnyInput& userInput, std::string& nextScene, Camera* camera) override;
	void deActivate(const Camera* camera) override;
protected:
	void doSetupScene() override;
};

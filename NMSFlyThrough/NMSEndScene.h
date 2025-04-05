#pragma once

#include "NMSScene.h"


/*
	An implementation of a Scene for the NMS game.
	Will be moved out of the engine to a different repo
*/
class NMSEndScene : public NMSScene
{
public:
	NMSEndScene(const Movie* movie);
	std::string name() const { return "The End"; }
	void activate(const std::string& previousScene, Camera* camera, unsigned int callCount) override;
	bool processUserCommands(const UserInput::AnyInput& userInput, std::string& nextScene, Camera* camera) override;
	void deActivate(const Camera* camera) override;
protected:
	void doSetupScene() override;
};


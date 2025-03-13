#pragma once

#include "NMSScene.h"

/*
	An implementation of a Scene for the NMS game.
	Will be moved out of the engine to a different repo
*/
struct NMSRopeScenePhysics : public ScenePhysicsState
{
	NMSRopeScenePhysics(Scene* owner)
		: ScenePhysicsState(owner) {}
	void setup() override;
	void variableTimeStep(OWUtils::Time::duration dt) override;
	void fixedTimeStep(std::string& nextSceneName, OWUtils::Time::duration dt) override;
	void interpolateRatio(const ScenePhysicsState* previousState, double multPrev,
		const ScenePhysicsState* currentState, double multCurr) override;
	bool processUserCommands(const UserInput::AnyInput& userInput,
		std::string& nextScene,
		Camera* camera) override;

	void copy(ScenePhysicsState* source) override;
	ScenePhysicsState* clone() override;
	glm::vec3 mCameraFocus = glm::vec3(0);
};

class NMSRopeScene : public NMSScene
{
public:
	NMSRopeScene(const Movie* movie);
	std::string name() const { return "Rope"; }
	void activate(const std::string& previousScene, ScenePhysicsState* state,
		Camera* camera, unsigned int callCount) override;
	void deActivate(const Camera* camera, ScenePhysicsState* state) override;
protected:
	void doSetupScene(ScenePhysicsState* state) override;
};


#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "NMSScene.h"

/*
	An implementation of a Scene for the NMS game.
	Will be moved out of the engine to a different repo
*/
struct NMSMainScenePhysics : public NMSWorldPhysicsState
{
	NMSMainScenePhysics(Scene* owner)
		: NMSWorldPhysicsState(owner) {}

	glm::vec3 mCameraPosition = glm::vec3(-1.48475, 1.77853, -0.553399);
	glm::vec3 mLookAt = glm::vec3(-0.799972, 1.20117, -0.10871);
	void setup() override;
	virtual void variableTimeStep(OWUtils::Time::duration dt) override;
	virtual void fixedTimeStep(std::string& nextSceneName, OWUtils::Time::duration dt) override;
	virtual void interpolateRatio(const ScenePhysicsState* previousState, double multPrev,
								  const ScenePhysicsState* currentState, double multCurr) override;
	bool processUserCommands(const UserInput::AnyInput& userInput, std::string& nextScene, 
							 Camera* camera) override;

	void copy(ScenePhysicsState* source) override;
	ScenePhysicsState* clone() override;
};

class NMSMainScene : public NMSScene
{
public:
	NMSMainScene(const Movie* movie);
	std::string name() const { return "Main"; }
	void activate(const std::string& previousScene, ScenePhysicsState* state, 
				  Camera* camera, unsigned int callCount) override;
	void deActivate(const Camera* camera, ScenePhysicsState* state) override;
protected:
	void doSetupScene(ScenePhysicsState* state) override;
};

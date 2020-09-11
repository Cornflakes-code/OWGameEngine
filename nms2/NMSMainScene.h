#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <Helpers/Shader.h>
#include <Renderables/BoundingBox.h>

#include "NMSScene.h"

struct NMSMainScenePhysics : public NMSWorldPhysicsState
{
	NMSMainScenePhysics(const Scene* owner)
		: NMSWorldPhysicsState(owner) {}

	glm::vec3 mCameraPosition = glm::vec3(-1.48475, 1.77853, -0.553399);
	glm::vec3 mLookAt = glm::vec3(-0.799972, 1.20117, -0.10871);
	virtual void variableTimeStep(OWUtils::Time::duration dt) override;
	virtual void fixedTimeStep(std::string& nextSceneName, OWUtils::Time::duration dt) override;
	virtual void interpolateRatio(const ScenePhysicsState* previousState, double multPrev,
								  const ScenePhysicsState* currentState, double multCurr) override;
	bool processUserCommands(const UserInput::AnyInput& userInput, std::string& nextScene, 
							 const glm::uvec2& screenSize, Camera* camera) override;


	void copy(ScenePhysicsState* source) override;
	ScenePhysicsState* clone() override;
};

class NoMansSky;
class Axis;
class NMSMainScene : public NMSScene
{
	AABB mWorldSpace;

	NoMansSky* mStarMap;
	Axis* mAxis;

public:
	NMSMainScene(const Movie* movie);
	virtual std::string name() const final { return NMSScene::mainSceneName(); }
	void doSetup(ScenePhysicsState* state) override;
	virtual void render(const ScenePhysicsState* state,
		const glm::mat4& proj, const glm::mat4& view) override;
	void activate(const std::string& previousScene, ScenePhysicsState* state, 
				  Camera* camera, unsigned int callCount) override;
	void deActivate(const std::string& previousScene, const Camera* camera, ScenePhysicsState* state) override;
};

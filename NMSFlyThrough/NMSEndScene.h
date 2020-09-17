#pragma once

#include "NMSScene.h"
#include <Renderables/SimpleVertexRender.h>

class Shader;
class TextBillboard;

/*
	An implementation of a Scene for the NMS game.
	Will be moved out of the engine to a different repo
*/
struct NMSEndScenePhysics : public ScenePhysicsState
{
	NMSEndScenePhysics(const Scene* owner)
		: ScenePhysicsState(owner) {}
	void variableTimeStep(OWUtils::Time::duration dt) override;
	void fixedTimeStep(std::string& nextSceneName, OWUtils::Time::duration dt) override;
	void interpolateRatio(const ScenePhysicsState* previousState, double multPrev,
		const ScenePhysicsState* currentState, double multCurr) override;
	bool processUserCommands(const UserInput::AnyInput& userInput, 
							 std::string& nextScene, 
							 Camera* camera) override;

	void copy(ScenePhysicsState* source) override;
	ScenePhysicsState* clone() override;

	float textPosition = 0.0;
};


class NMSEndScene : public NMSScene
{
	Shader* mShader;
	SimpleVertexRender mText;
public:
	NMSEndScene(const Movie* movie);
	virtual std::string name() const final { return NMSScene::endSceneName(); }
	void doSetup(ScenePhysicsState* state) override;
	virtual void render(const ScenePhysicsState* state,
		const glm::mat4& proj, const glm::mat4& view) override;
	void activate(const std::string& previousScene, ScenePhysicsState* state, 
				  Camera* camera, unsigned int callCount) override;
	void deActivate(const std::string& previousScene, const Camera* camera, ScenePhysicsState* state) override;
};


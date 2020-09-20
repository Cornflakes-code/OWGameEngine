#pragma once

#include <limits>

#include <glm/glm.hpp>
#include <Renderables/BoundingBox.h>
#include <Renderables/MovingText.h>

#include "NMSScene.h"
/*
	An implementation of a Scene for the NMS game.
	Will be moved out of the engine to a different repo
*/

struct NMSSplashScenePhysics: public NMSWorldPhysicsState
{
	NMSSplashScenePhysics(const Scene* owner)
		: NMSWorldPhysicsState(owner) {}

	virtual void variableTimeStep(OWUtils::Time::duration dt) override;
	virtual void fixedTimeStep(std::string& nextSceneName, OWUtils::Time::duration dt) override;
	virtual void interpolateRatio(const ScenePhysicsState* previousState, double multPrev,
		const ScenePhysicsState* currentState, double multCurr) override;
	bool processUserCommands(const UserInput::AnyInput& userInput, std::string& nextScene, 
							 Camera* camera) override;

	ScenePhysicsState* clone() override;
	void copy(ScenePhysicsState* source) override;
	void clear() override;

	static AABB mWindowBounds;
	static OWUtils::Float mSpeed;

	MovingText mWelcome;
	MovingText mEnjoy;
};

class Axis;
class FullScreen;

class NMSSplashScene : public NMSScene
{
	
	FullScreen* mFullScreen;
	Axis* mAxis;
	SimpleVertexRender mCircle;
public:
	NMSSplashScene(const Movie* movie);
	virtual std::string name() const final { return NMSScene::splashSceneName(); }
	void doSetup(ScenePhysicsState* state) override;
	virtual void render(const ScenePhysicsState* state,
		const glm::mat4& proj, const glm::mat4& view) override;
	void activate(const std::string& previousScene, ScenePhysicsState* state,
				  Camera* camera, unsigned int callCount) override;
	void deActivate(const std::string& previousScene, const Camera* camera, ScenePhysicsState* state) override;
private:
};


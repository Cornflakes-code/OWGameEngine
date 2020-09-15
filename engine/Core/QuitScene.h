#pragma once

#include "Scene.h"
/*
	A core Scene required as a placeholder for the Game Loop implementation in Movie.cpp
*/
struct QuitScenePhysics: public ScenePhysicsState
{
	QuitScenePhysics(const Scene* owner)
		: ScenePhysicsState(owner) {}
	void variableTimeStep(OWUtils::Time::duration dt) override {}
	void fixedTimeStep(std::string& nextSceneName, OWUtils::Time::duration dt) override {}
	void interpolateRatio(const ScenePhysicsState* previousState, double multPrev,
		const ScenePhysicsState* currentState, double multCurr) override {}
	bool processUserCommands(const UserInput::AnyInput& userInput, std::string& nextScene, 
							 const glm::uvec2& screenSize, Camera* camera) override;

	void copy(ScenePhysicsState* source) override;
	ScenePhysicsState* clone() override;
};


class QuitScene : public Scene
{
	// The framework requires a scene with a name of OWUtils::quitSceneName().
	// It will be the last scene loaded and can (not must!) be overridden to clear resource.
public:
	QuitScene(const Movie* movie);
	virtual std::string name() const final { return quitSceneName(); }
	void doSetup(ScenePhysicsState* state) override;
	virtual void render(const ScenePhysicsState* state,
		const glm::mat4& proj, const glm::mat4& view) override;
	void activate(const std::string& previousScene, ScenePhysicsState* state,
				  Camera* camera, unsigned int callCount) override;
	void deActivate(const std::string& previousScene, const Camera* camera, ScenePhysicsState* state) override;
};
